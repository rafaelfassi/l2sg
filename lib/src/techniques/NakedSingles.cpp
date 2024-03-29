#include "Grid.h"
#include "Solver.h"
#include "Utils.h"

namespace l2sg::solver::techniques
{

// Encompasses Full House/Last Digit
bool nakedSingles(Grid &pGrid, Logs *logs, bool *check)
{
    bool changedOverall(false);
    bool changed;

    do
    {
        changed = false;
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                if (auto &cell = pGrid.getCell(i, j); cell.notesCount() == 1)
                {
                    const auto note = utils::bitset_it(cell.getNotes()).front() + 1;
                    cell.setValue(note);
                    pGrid.clearNotesCascade(i, j, note, nullptr, check);
                    if (check && !(*check))
                        return false;

                    if (logs)
                    {
                        ScopedLog log(logs, Technique::NakedSingles);
                        log.addCellLog(i, j, CellAction::AppliedValue, note);
                    }
                    changed = true;
                }
            }
        }
        changedOverall |= changed;
    } while (changed);

    if (check)
        *check = true;
    return changedOverall;
}

} // namespace l2sg::solver::techniques
