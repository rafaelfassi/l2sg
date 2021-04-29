#include "Grid.h"

namespace sudoku::solver::techniques
{

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
                    std::vector<int> notes;
                    cell.getNotesLst(notes);
                    if (check)
                    {
                        if (!pGrid.isAllowedValue(i, j, notes.front()))
                        {
                            *check = false;
                            return;
                        }
                    }
                    cell.setValue(notes.front());
                    if (!pGrid.clearNotesCascade(i, j, notes.front()) && check)
                    {
                        *check = false;
                        return;
                    }
                    changed = true;
                    // std::cout << "Solitary >> Lin: " << i+1 << " - Col: " << j+1 << " - Num: " << x <<
                    // std::endl; pGrid.dump();
                }
            }
        }

    } while (changed);

    if (check)
    {
        *check = true;
    }
}

} // namespace sudoku::solver::techniques
