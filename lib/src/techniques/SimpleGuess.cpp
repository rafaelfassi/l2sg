#include "Grid.h"
#include "Solver.h"
#include "Utils.h"
#include <iostream>
#include <map>

namespace l2sg::solver::techniques
{

bool simpleGuess(Grid &pGrid, Logs *logs)
{
    using CellsRank = std::multimap<int, std::pair<int, int>>;
    CellsRank rankedCells;

    const auto checkConflict = [&](int row, int col, int note, bool &isFull) -> bool
    {
        Grid grid(pGrid);
        grid.setValue(row, col, note);
        grid.clearNotesCascade(row, col, note, nullptr);

        bool ok;
        nakedSingles(grid, nullptr, &ok);
        if (ok && grid.checkAllNotes())
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
            bool isFull(false);
            if (checkConflict(r, c, nIdx + 1, isFull))
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
                    ScopedLog log(logs, Technique::SimpleGuess);
                    log.addCellLog(r, c, CellAction::AppliedValue, goodValue);
                }
                return true;
            }
        }
    }

    return false;
}

} // namespace l2sg::solver::techniques
