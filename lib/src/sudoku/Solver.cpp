#include "Solver.h"
#include <iostream>
#include <map>
#include <string>

namespace sudoku
{

Level solver::solveLevel(Grid &pGrid, Level maxLevel)
{
    int level(LEV_0_LOGIC);
    std::string notesSignature;
    std::string newNotesSignature;
    bool loop(true);

    while (true)
    {
        techniques::nakedSingles(pGrid);

        if (pGrid.isFull())
            break;

        if (techniques::hiddenSingles(pGrid))
            continue;

        if (level < LEV_1_LOGIC)
            level = LEV_1_LOGIC;

        if (techniques::lockedCandidates(pGrid))
            continue;

        if (techniques::nakedMulti(pGrid, 2))
            continue;

        if (techniques::hiddenMulti(pGrid, 2))
            continue;

        if (level < LEV_2_LOGIC)
            level = LEV_2_LOGIC;

        if (techniques::nakedMulti(pGrid))
            continue;

        if (techniques::hiddenMulti(pGrid))
            continue;

        if (techniques::xWings(pGrid))
            continue;

        if (techniques::swordfish(pGrid))
            continue;

        level = LEV_3_GUESS;
        if (solveByGuesses(pGrid) != 1)
        {
            level = LEV_UNKNOWN;
        }
        break;
    }

    return static_cast<Level>(level);
}

int solver::solveByGuesses(Grid &pGrid, int maxSolutions)
{
    using CellsRank = std::multimap<int, std::pair<int, int>>;
    std::vector<Grid> solutions;
    CellsRank guessesCellsRank;
    Grid grid = pGrid;

    const std::function<void(CellsRank::const_iterator)> evalCell = [&](CellsRank::const_iterator it) {
        if (solutions.size() >= maxSolutions)
            return;

        if (it != guessesCellsRank.end())
        {
            const int lin = it->second.first;
            const int col = it->second.second;
            const auto &cell = grid.getCell(lin, col);
            ++it;
            if (cell.getValue() > 0)
            {
                evalCell(it);
            }
            else
            {
                int candidate(0);
                while ((candidate = cell.getNextNote(candidate)))
                {
                    Grid savedPoint = grid;
                    grid.setValue(lin, col, candidate);
                    if (grid.clearNotesCascade(lin, col, candidate))
                    {
                        bool noConflicts(true);
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