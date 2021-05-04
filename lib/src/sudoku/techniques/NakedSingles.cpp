#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

// Encompasses Full House/Last Digit
void nakedSingles(Grid &pGrid, bool *check)
{
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
                    const auto note = cell.getNextNote(0);
                    cell.setValue(note);
                    pGrid.clearNotesCascade(i, j, note, check);
                    if (check && !(*check))
                    {
                        return;
                    }
                    changed = true;
                }
            }
        }
    } while (changed);
}

} // namespace sudoku::solver::techniques
