#include "Generator.h"
#include "Solver.h"

#include <chrono>
#include <iostream>
#include <random>
#include <stack>

namespace l2sg
{

std::seed_seq genRanSeed()
{
    std::random_device rd;
    const auto time = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    return std::seed_seq{static_cast<uint64_t>(rd()), time, static_cast<uint64_t>(rd())};
}

void generator::generateRandomFullGrid(Grid &_grid)
{
    constexpr int clues(15);
    std::seed_seq seed(genRanSeed());
    std::mt19937 rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> randNote(1, 9);
    std::vector<std::pair<int, int>> emptyCells;

    const auto addRandValue = [&]()
    {
        std::uniform_int_distribution<std::mt19937::result_type> randAdd(0, emptyCells.size() - 1);
        const int pos = randAdd(rng);
        const auto &rc = emptyCells[pos];
        int n(randNote(rng));
        int i(0);

        do
        {
            if(++i > 9)
                return;

            if (++n > 9)
                n = 1;

        } while (!_grid.isAllowedValue(rc.first, rc.second, n));

        _grid.setValue(rc.first, rc.second, n);
        emptyCells[pos] = emptyCells.back();
        emptyCells.pop_back();
    };

    emptyCells.reserve(81);

    do
    {
        _grid = Grid();
        emptyCells.clear();
        for (int r = 0; r < 9; ++r)
        {
            for (int c = 0; c < 9; ++c)
            {
                emptyCells.emplace_back(r, c);
            }
        }

        for (int i = 0; i < clues; ++i)
        {
            addRandValue();
        }

        _grid.fillNotes();

    } while (solver::techniques::bruteForce(_grid, 1, nullptr) == 0);
}

void generator::generate(Grid &_grid, const std::function<int(Grid &)> _solve, bool _symmetric)
{
    using ClueTp = std::tuple<int, int, int>;

    constexpr int maxRemoval(81 - 17); // The minimum valid sudoku must have at least 17 clues
    constexpr int minRemoval(81 - 40); // Don't generate puzzles with more than 40 clues
    int iniRemoval(maxRemoval);
    std::mt19937 rng;
    std::vector<ClueTp> cluesToRemove;
    std::stack<ClueTp> removedClues;
    Grid fullGrid;

    const auto hasClueToRemove = [&]() -> bool
    {
        if (!_symmetric || cluesToRemove.empty())
        {
            return !cluesToRemove.empty();
        }
        else
        {
            if (cluesToRemove.size() > 1)
            {
                return true;
            }
            else
            {
                const auto clue = cluesToRemove.at(0);
                return (std::get<0>(clue) == 4) && (std::get<1>(clue) == 4);
            }
        }
    };

    const auto removeOneClue = [&]()
    {
        if (hasClueToRemove())
        {
            std::uniform_int_distribution<std::mt19937::result_type> randRem(0, cluesToRemove.size() - 1);
            const int pos = randRem(rng);
            const auto clue = cluesToRemove[pos];

            _grid.setValue(std::get<0>(clue), std::get<1>(clue), 0);
            removedClues.push(clue);
            cluesToRemove[pos] = cluesToRemove.back();
            cluesToRemove.pop_back();

            if (_symmetric)
            {
                int r = (8 - std::get<0>(clue));
                int c = (8 - std::get<1>(clue));
                if ((r != 4) || (c != 4))
                {
                    auto it = std::find_if(cluesToRemove.begin(), cluesToRemove.end(),
                                           [r, c](const ClueTp &cl)
                                           { return (std::get<0>(cl) == r && std::get<1>(cl) == c); });
                    if (it != cluesToRemove.end())
                    {
                        _grid.setValue(std::get<0>(*it), std::get<1>(*it), 0);
                        removedClues.push(*it);
                        *it = cluesToRemove.back();
                        cluesToRemove.pop_back();
                    }
                }
            }
        }
    };

    const auto restoreRemovedClue = [&]()
    {
        if (!removedClues.empty())
        {
            const auto clue(removedClues.top());
            _grid.setValue(std::get<0>(clue), std::get<1>(clue), std::get<2>(clue));
            removedClues.pop();

            if (_symmetric && !removedClues.empty() && ((std::get<0>(clue) != 4) || (std::get<1>(clue) != 4)))
            {
                const auto &clueSymm(removedClues.top());
                if (std::get<0>(clueSymm) != 4 || std::get<1>(clueSymm) != 4)
                {
                    _grid.setValue(std::get<0>(clueSymm), std::get<1>(clueSymm), std::get<2>(clueSymm));
                    removedClues.pop();
                }
            }
        }
    };

    const auto resetAll = [&]()
    {
        std::seed_seq seed(genRanSeed());
        rng.seed(seed);
        fullGrid = Grid();
        generateRandomFullGrid(fullGrid);
        _grid = fullGrid;

        cluesToRemove.clear();
        while (!removedClues.empty())
            removedClues.pop();

        for (int r = 0; r < 9; ++r)
            for (int c = 0; c < 9; ++c)
                cluesToRemove.emplace_back(std::make_tuple(r, c, _grid.getValue(r, c)));

        while (removedClues.size() < iniRemoval)
            removeOneClue();
    };

    const auto flip = [&]()
    {
        cluesToRemove.clear();
        while (!removedClues.empty())
            removedClues.pop();

        std::vector<std::pair<int, int>> tmpRemovedClues;
        tmpRemovedClues.reserve(maxRemoval);

        for (int r = 0; r < 9; ++r)
        {
            for (int c = 0; c < 9; ++c)
            {
                if (_grid.getValue(r, c) == 0)
                {
                    const auto v = fullGrid.getValue(r, c);
                    _grid.setValue(r, c, v);
                    cluesToRemove.emplace_back(std::make_tuple(r, c, v));
                }
                else
                {
                    tmpRemovedClues.emplace_back(r, c);
                }
            }
        }

        for (const auto [r, c] : tmpRemovedClues)
        {
            if (_grid.getValue(r, c))
            {
                _grid.setValue(r, c, 0);
                removedClues.emplace(std::make_tuple(r, c, fullGrid.getValue(r, c)));

                if (_symmetric && ((r != 4) || (c != 4)))
                {
                    int rSymm = 8 - r;
                    int cSymm = 8 - c;
                    _grid.setValue(rSymm, cSymm, 0);
                    removedClues.emplace(std::make_tuple(rSymm, cSymm, fullGrid.getValue(rSymm, cSymm)));
                }
            }
        }

        while (removedClues.size() < iniRemoval)
            removeOneClue();

        if (--iniRemoval < minRemoval)
            iniRemoval = maxRemoval;
    };

    size_t flippedCount(0);
    bool everRemoved(false);
    int solveRes(-1);

    resetAll();

    do
    {
        if (removedClues.size() > maxRemoval)
        {
            restoreRemovedClue();
            restoreRemovedClue();
            removeOneClue();
        }
        else if (removedClues.size() < minRemoval)
        {
            removeOneClue();
        }

        if (!hasClueToRemove())
        {
            if (++flippedCount == 30000)
            {
                flippedCount = 0;
                resetAll();
            }
            else
            {
                flip();
            }
            everRemoved = false;
        }

        Grid solvingGrid(_grid);
        solveRes = _solve(solvingGrid);
        if (solveRes < 0)
        {
            removeOneClue();
            everRemoved = true;
        }
        else if (solveRes > 0)
        {
            restoreRemovedClue();
            if (everRemoved)
            {
                removeOneClue();
            }
        }

    } while (solveRes != 0);
}

void generator::generateByLevel(Grid &_grid, Level _targetLevel, bool _symmetric)
{
    generate(
        _grid,
        [_targetLevel](Grid &solvingGrid) -> int
        {
            solvingGrid.fillNotes();
            Level level = solver::solve(solvingGrid, nullptr, _targetLevel);
            if (level < _targetLevel)
            {
                return -1;
            }
            else if (level > _targetLevel)
            {
                return 1;
            }
            return 0;
        },
        _symmetric);
}

void generator::generateByTechnique(Grid &_grid, solver::Technique _technique, bool _symmetric)
{
    static const solver::TechniqueSet baseAllowedTechniques{
        //
        solver::Technique::NakedSingles,
        solver::Technique::NakedPair,
        solver::Technique::NakedTriple,
        solver::Technique::HiddenSingles,
        solver::Technique::HiddenPair,
        solver::Technique::HiddenTriple,
        solver::Technique::LockedCandidatesType1,
        solver::Technique::LockedCandidatesType2,
        solver::Technique::XWings,
        solver::Technique::Swordfish,
        solver::Technique::XYWing,
        solver::Technique::WWing,
        solver::Technique::Skyscraper,
        solver::Technique::TwoStringKite
        //
    };

    solver::TechniqueSet allowedTechniques(baseAllowedTechniques);
    // Ensures the requested technique will be in the list.
    allowedTechniques.insert(_technique);

    generate(
        _grid,
        [_technique, &allowedTechniques](Grid &solvingGrid) -> int
        {
            solver::TechniqueSet usedTechniques;
            solvingGrid.fillNotes();
            if (solver::solveByTechniques(solvingGrid, allowedTechniques, &usedTechniques))
            {
                if (usedTechniques.find(_technique) != usedTechniques.end())
                    return 0;
                else
                    return -1;
            }
            else
            {
                return 1;
            }
        },
        _symmetric);
}

} // namespace l2sg