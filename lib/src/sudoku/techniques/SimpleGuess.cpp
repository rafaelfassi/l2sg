#include "Grid.h"
#include "Solver.h"
#include "Utils.h"
#include <iostream>
#include <map>

namespace sudoku::solver::techniques
{

bool simpleGuess(Grid &pGrid, Logs *logs)
{
    using CellsRank = std::multimap<int, std::pair<int, int>>;
    CellsRank rankedCells;

    const auto findConflict = [&](int row, int col, int note, bool &isFull) -> bool
    {
        Grid grid(pGrid);
        grid.setValue(row, col, note);
        grid.clearNotesCascade(row, col, note, nullptr);

        bool ok;
        nakedSingles(grid, nullptr, &ok);
        if (ok)
        {
            isFull = grid.isFull();
            return false;
        }
        return true;
    };

    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            const auto &cell = pGrid.getCell(r, c);
            if (cell.hasAnyNote())
            {
                // Order by the ones with less candidates
                rankedCells.insert(std::make_pair(cell.notesCount(), std::make_pair(r, c)));
            }
        }
    }

    pGrid.resetSummary();

    for (const auto &it : rankedCells)
    {
        const auto &[r, c] = it.second;
        const auto notes(pGrid.getNotes(r, c));
        const auto targetConflictCount(notes.count() - 1);
        size_t conflictCount(0);
        int goodValue(0);

        for (const auto nIdx : utils::bitset_it(notes))
        {
            bool isFull;
            if (findConflict(r, c, nIdx + 1, isFull))
                ++conflictCount;
            else if (isFull || (goodValue == 0))
                goodValue = nIdx + 1;
            else
                break;

            if (isFull || (goodValue && (conflictCount == targetConflictCount)))
            {
                pGrid.setValue(r, c, goodValue);
                pGrid.clearNotesCascade(r, c, goodValue);

                if (logs)
                {
                    Log log(Technique::SimpleGuess);
                    log.cellLogs.emplace_back(r, c, CellAction::SetValue, goodValue);
                    logs->push_back(std::move(log));
                }
                return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
