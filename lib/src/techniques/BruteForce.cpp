#include "Grid.h"
#include "Solver.h"
#include "Utils.h"
#include <iostream>
#include <map>

namespace l2sg::solver::techniques
{

int bruteForce(Grid &pGrid, int maxSolutions, Logs *logs)
{
    using CellsRank = std::multimap<int, std::pair<int, int>>;
    std::vector<Grid> solutions;
    CellsRank guessesCellsRank;
    pGrid.resetSummary();
    Grid grid = pGrid;

    const std::function<void(CellsRank::const_iterator)> evalCell = [&](CellsRank::const_iterator it)
    {
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
                    grid.clearNotesCascade(lin, col, candidateIdx + 1, nullptr, &noConflicts);
                    if (noConflicts)
                    {
                        techniques::nakedSingles(grid, nullptr, &noConflicts);
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

    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            const auto &cell = grid.getCell(r, c);
            if (cell.hasAnyNote())
            {
                // Order by the ones with less candidates
                guessesCellsRank.insert(std::make_pair(cell.notesCount(), std::make_pair(r, c)));
            }
        }
    }

    evalCell(guessesCellsRank.begin());

    if (!solutions.empty())
    {
        pGrid = solutions.front();

        if (logs)
        {
            Log log(Technique::BruteForce);
            logs->push_back(std::move(log));
        }
    }

    return solutions.size();
}

} // namespace l2sg::solver::techniques
