#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

void xWings(Grid &pGrid)
{
    std::bitset<9> rows[9][9];
    std::bitset<9> cols[9][9];

    const auto fillDataFunc = [&](int l, int c, int b) -> bool {
        Cell &cell = pGrid.getCell(l, c);
        int note(0);
        while ((note = cell.getNextNote(note)))
        {
            const int noteIdx = note - 1;
            rows[l][noteIdx].set(c, true);
            cols[c][noteIdx].set(l, true);
        }
        return true;
    };

    const auto processDataFunc = [&](int i, int v, const int type, const auto &dataSet) {
        const int vIdx = v - 1;

        // A number was found only 2 times in a row?
        if (dataSet[i][vIdx].count() == 2)
        {
            // Try to find another row in which the same number appers 2 times and in the same columns.
            for (int i2 = i + 1; i2 < 9; ++i2)
            {
                if (dataSet[i][vIdx] == dataSet[i2][vIdx])
                {
                    int j(-1);
                    while ((j = utils::getNext(dataSet[i][vIdx], j)) != -1)
                    {
                        if (type == Grid::T_LINE)
                            pGrid.clearColNotes(j, v, [i, i2](int r) { return (r != i) && (r != i2); });
                        else if (type == Grid::T_COLUMN)
                            pGrid.clearRowNotes(j, v, [i, i2](int c) { return (c != i) && (c != i2); });
                    }
                    break;
                }
            }
        }
    };

    pGrid.forAllCells(fillDataFunc);

    // As processDataFunc will look for a ahead row to match, 'i' may iterate from 0 to 7
    for (int i = 0; i < 8; ++i)
    {
        for (int v = 1; v < 10; ++v)
        {
            processDataFunc(i, v, Grid::T_LINE, rows);
            processDataFunc(i, v, Grid::T_COLUMN, cols);
        }
    }
}

} // namespace sudoku::solver::techniques
