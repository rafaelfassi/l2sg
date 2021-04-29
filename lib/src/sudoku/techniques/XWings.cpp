#include "Grid.h"

namespace sudoku::solver::techniques
{

void xWings(Grid &pGrid)
{
    std::bitset<9> rows[9][9];
    std::bitset<9> cols[9][9];
    std::vector<int> notes;
    constexpr int sz = sizeof(rows) / sizeof(int);

    const std::function<bool(int, int, int)> fillDataFunc = [&](int l, int c, int b) -> bool {
        Cell &cell = pGrid.getCell(l, c);

        notes.clear();
        cell.getNotesLst(notes);

        for (const auto note : notes)
        {
            const int noteIdx = note - 1;
            rows[l][noteIdx].set(c, true);
            cols[c][noteIdx].set(l, true);
        }

        return true;
    };

    pGrid.forAllCells(fillDataFunc);

    for (int i = 0; i < 9; ++i)
    {
        for (int v = 1; v < 10; ++v)
        {
            const int vIdx = v - 1;

            // A number was found only 2 times in a row?
            if (rows[i][vIdx].count() == 2)
            {
                bool foundPair(false);
                // Try to find another row in which the same number appers 2 and in the same columns.
                for (int i2 = i + 1; !foundPair && i2 < 9; ++i2)
                {
                    if (rows[i][vIdx] == rows[i2][vIdx])
                    {
                        for (int j = 0; j < 9; ++j)
                        {
                            if (rows[i][vIdx].test(j))
                            {
                                pGrid.clearColNotes(j, v, [i, i2](int r) { return (r != i) && (r != i2); });
                                foundPair = true;
                            }
                        }
                    }
                }
            }

            if (cols[i][vIdx].count() == 2)
            {
                bool foundPair(false);
                for (int i2 = i + 1; !foundPair && i2 < 9; ++i2)
                {
                    if (cols[i][vIdx] == cols[i2][vIdx])
                    {
                        for (int j = 0; j < 9; ++j)
                        {
                            if (cols[i][vIdx].test(j))
                            {
                                pGrid.clearRowNotes(j, v, [i, i2](int c) { return (c != i) && (c != i2); });
                                foundPair = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace sudoku::solver::techniques
