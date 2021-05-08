#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

// Encompasses Full House/Last Digit
bool nakedSingles(Grid &pGrid, bool *check)
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
                    pGrid.clearNotesCascade(i, j, note, check);
                    if (check && !(*check))
                    {
                        return false;
                    }
                    changed = true;
                }
            }
        }
        changedOverall |= changed;
    } while (changed);

    return changedOverall;
}

} // namespace sudoku::solver::techniques
