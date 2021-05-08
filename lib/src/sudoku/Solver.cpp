#include "Solver.h"
#include "Utils.h"
#include <iostream>
#include <map>
#include <string>

#define SWITCH_LEVEL(p_newLevel, p_level, p_maxLevel)                                                        \
    if (p_level == p_maxLevel)                                                                               \
    {                                                                                                        \
        p_level = LEV_UNKNOWN;                                                                               \
        break;                                                                                               \
    }                                                                                                        \
    if (p_level < p_newLevel)                                                                                \
        p_level = p_newLevel;

namespace sudoku
{

Level solver::solveLevel(Grid &pGrid, Level maxLevel)
{
    int level(LEV_0_LOGIC);

    while (true)
    {
        techniques::nakedSingles(pGrid);

        if (pGrid.isFull())
            break;

        if (techniques::hiddenSingles(pGrid))
            continue;

        SWITCH_LEVEL(LEV_1_LOGIC, level, maxLevel)

        if (techniques::lockedCandidates(pGrid))
            continue;

        if (techniques::hiddenMulti(pGrid, 2, 2))
            continue;

        if (techniques::nakedMulti(pGrid, 2, 2))
            continue;

        SWITCH_LEVEL(LEV_2_LOGIC, level, maxLevel)

        if (techniques::hiddenMulti(pGrid, 3))
            continue;

        if (techniques::nakedMulti(pGrid, 3))
            continue;

        if (techniques::xWings(pGrid))
            continue;

        // Swordfish and Jellyfish
        if (techniques::basicFish(pGrid, 3, 4))
            continue;

        SWITCH_LEVEL(LEV_3_GUESS, level, maxLevel)

        if (solveByGuesses(pGrid) == 1)
            break;

        level = LEV_UNKNOWN;
        break;
    }

    return static_cast<Level>(level);
}

int solver::solveByGuesses(Grid &pGrid, int maxSolutions)
{
    using CellsRank = std::multimap<int, std::pair<int, int>>;
    std::vector<Grid> solutions;
    CellsRank guessesCellsRank;
    pGrid.resetSummary();
    Grid grid = pGrid;

    const std::function<void(CellsRank::const_iterator)> evalCell = [&](CellsRank::const_iterator it) {
        if (solutions.size() >= maxSolutions)
            return;

        if (it != guessesCellsRank.end())
        {
            const auto [lin, col] = it->second;
            const auto &cell = grid.getCell(lin, col);
            ++it;
            if (cell.getValue() > 0)
            {
                evalCell(it);
            }
            else
            {
                for (const auto candidateIdx : utils::bitset_it(cell.getNotes()))
                {
                    Grid savedPoint = grid;
                    bool noConflicts(true);
                    grid.setValue(lin, col, candidateIdx + 1);
                    grid.clearNotesCascade(lin, col, candidateIdx + 1, &noConflicts);
                    if (noConflicts)
                    {
                        techniques::nakedSingles(grid, &noConflicts);
                        if (noConflicts)
                        {
                            evalCell(it);
                            // This may avoid an extra iteration of the loop.
                            if (solutions.size() >= maxSolutions)
                                break;
                        }
                    }

                    // When the function hits here, either the guess failed, or the puzzle is done.
                    // If the puzzle is done, the solutions are already placed in the solutions' vector.
                    // If the guess failed, the previous grid must be restored to try another candidate.
                    grid = savedPoint;
                }
            }
        }
        else
        {
            solutions.push_back(grid);
        }
    };

    for (int l = 0; l < 9; ++l)
    {
        for (int c = 0; c < 9; ++c)
        {
            const auto &cell = grid.getCell(l, c);
            if (cell.hasAnyNote())
            {
                // Order by the ones with less candidates
                guessesCellsRank.insert(std::make_pair(cell.notesCount(), std::make_pair(l, c)));
            }
        }
    }

    evalCell(guessesCellsRank.begin());

    if (!solutions.empty())
    {
        pGrid = solutions.front();
    }

    return solutions.size();
}

} // namespace sudoku