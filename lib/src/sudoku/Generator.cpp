#include "Generator.h"
#include "Solver.h"

#include <iostream>
#include <random>
#include <unordered_set>
#include <stack>

namespace sudoku
{

void generator::generateRandomFullGrid(Grid &grid)
{
    constexpr int clues(15);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randNote(1, 9);
    std::vector<std::pair<int, int>> emptyCells;

    const auto addRandValue = [&](Grid &grid)
    {
        std::uniform_int_distribution<std::mt19937::result_type> randAdd(0, emptyCells.size() - 1);

        int pos = randAdd(rng);
        const auto &rc = emptyCells[pos];

        int n(0);
        size_t cnt(0);
        do
        {
            n = randNote(rng);
            ++cnt;
        } while (!grid.isAllowedValue(rc.first, rc.second, n) && (cnt < 10));

        if (cnt < 10)
        {
            grid.setValue(rc.first, rc.second, n);
            emptyCells[pos] = emptyCells.back();
            emptyCells.pop_back();
        }
    };

    emptyCells.reserve(81);

    do
    {
        grid = Grid();
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
            addRandValue(grid);
        }

        // grid.dump(Grid::D_VALUES);

        grid.fillNotes();
    } while (solver::techniques::bruteForce(grid, 1, nullptr) != 1);

    // grid.dump(Grid::D_VALUES);
}

void generator::generate(Grid &grid, const std::function<int(Grid &)> _solve, bool symmetric)
{
    using ClueTp = std::tuple<int, int, int>;

    const int iniRemoval(symmetric ? 20 : 40); // For symmetric, each removal will remove 2 clues
    constexpr int maxRemoval(81 - 17);
    std::mt19937 rng(std::random_device{}());
    std::vector<ClueTp> cluesToRemove;
    std::stack<ClueTp> removedClues;
    Grid fullGrid;

    const auto generateFullGrid = [&fullGrid, &rng]()
    {
        fullGrid = Grid();
        generateRandomFullGrid(fullGrid);
        rng.seed(std::random_device{}());
    };

    const auto removeOneClue = [&](Grid &grid)
    {
        if (!cluesToRemove.empty())
        {
            std::uniform_int_distribution<std::mt19937::result_type> randRem(0, cluesToRemove.size() - 1);

            const int pos = randRem(rng);
            const auto clue = cluesToRemove[pos];
            grid.setValue(std::get<0>(clue), std::get<1>(clue), 0);
            removedClues.push(clue);
            cluesToRemove[pos] = cluesToRemove.back();
            cluesToRemove.pop_back();

            if (symmetric)
            {
                int r = (8 - std::get<0>(clue));
                int c = (8 - std::get<1>(clue));
                if (r != 4 || c != 4)
                {
                    auto it = std::find_if(cluesToRemove.begin(), cluesToRemove.end(),
                                           [r, c](const ClueTp &cl)
                                           { return (std::get<0>(cl) == r && std::get<1>(cl) == c); });
                    if (it != cluesToRemove.end())
                    {
                        grid.setValue(std::get<0>(*it), std::get<1>(*it), 0);
                        removedClues.push(*it);
                        *it = cluesToRemove.back();
                        cluesToRemove.pop_back();
                    }
                }
            }
        }
    };

    const auto restoreRemovedClue = [&](Grid &grid)
    {
        if (!removedClues.empty())
        {
            const auto clue(removedClues.top());
            grid.setValue(std::get<0>(clue), std::get<1>(clue), std::get<2>(clue));
            removedClues.pop();

            if (symmetric && !removedClues.empty() && (std::get<0>(clue) != 4 || std::get<1>(clue) != 4))
            {
                const auto &clueSymm(removedClues.top());
                if (std::get<0>(clueSymm) != 4 || std::get<1>(clueSymm) != 4)
                {
                    grid.setValue(std::get<0>(clueSymm), std::get<1>(clueSymm), std::get<2>(clueSymm));
                    removedClues.pop();
                }
            }
        }
    };

    const auto resetGrid = [&](Grid &grid)
    {
        grid = fullGrid;
        cluesToRemove.clear();
        while (!removedClues.empty())
            removedClues.pop();

        for (int r = 0; r < 9; ++r)
            for (int c = 0; c < 9; ++c)
                cluesToRemove.emplace_back(std::make_tuple(r, c, grid.getValue(r, c)));

        for (int i = 0; i < iniRemoval; ++i)
            removeOneClue(grid);
    };

    //size_t solveCount(0);
    size_t resetCount(0);
    //size_t resetFullGridCount(0);

    // generateRandomFullGrid(fullGrid);
    generateFullGrid();

    int solveRes;
    do
    {
        if (cluesToRemove.empty())
        {
            if (++resetCount == 5000)
            {
                generateFullGrid();
                resetCount = 0;
                //++resetFullGridCount;
            }
            resetGrid(grid);
        }

        Grid solvingGrid(grid);
        solveRes = _solve(solvingGrid);
        if (solveRes < 0)
            removeOneClue(grid);
        else if (solveRes > 0)
            restoreRemovedClue(grid);

        //++solveCount;

    } while (solveRes != 0);

    // grid.dump(Grid::D_VALUES);

    // std::cout << "solveCount: " << solveCount << std::endl;
    // std::cout << "resetFullGridCount: " << resetFullGridCount << std::endl;
}

void generator::generateByLevel(Grid &grid, Level targetLevel)
{
    generate(grid,
             [targetLevel](Grid &solvingGrid) -> int
             {
                 solvingGrid.fillNotes();
                 Level level = solver::solve(solvingGrid, nullptr, targetLevel);
                 if (level < targetLevel)
                 {
                     return -1;
                 }
                 else if (level > targetLevel)
                 {
                     return 1;
                 }
                 return 0;
             });
}

void generator::generateByTechnique(Grid &grid, solver::Technique technique)
{
    static const std::unordered_set<solver::Technique> allowedTechniques{
        //
        solver::Technique::NakedSingles,
        solver::Technique::NakedPair,
        solver::Technique::NakedTriple,
        solver::Technique::NakedQuadruple,
        solver::Technique::HiddenSingles,
        solver::Technique::HiddenPair,
        solver::Technique::HiddenTriple,
        solver::Technique::HiddenQuadruple,
        solver::Technique::LockedCandidatesType1,
        solver::Technique::LockedCandidatesType2,
        solver::Technique::XWings,
        solver::Technique::Swordfish,
        solver::Technique::Jellyfish,
        solver::Technique::XYWing,
        solver::Technique::WWing,
        solver::Technique::Skyscraper,
        solver::Technique::TwoStringKite
        //
    };
    generate(grid,
             [technique](Grid &solvingGrid) -> int
             {
                 std::unordered_set<solver::Technique> usedTechniques;
                 solvingGrid.fillNotes();
                 if (solver::solveByTechniques(solvingGrid, allowedTechniques, usedTechniques))
                 {
                     if (usedTechniques.find(technique) != usedTechniques.end())
                         return 0;
                     else
                         return -1;
                 }
                 else
                 {
                     return 1;
                 }
             });
}

} // namespace sudoku