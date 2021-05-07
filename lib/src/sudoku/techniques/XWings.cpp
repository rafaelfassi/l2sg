#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

bool xWings(Grid &pGrid)
{
    const auto &summary(pGrid.getSummary());

    const auto processDataFunc = [&](int i, int v, const int type, const auto &dataSet) -> bool {
        const int vIdx = v - 1;
        // Try to find another row in which the same number appers 2 times and in the same columns.
        for (int i2 = i + 1; i2 < 9; ++i2)
        {
            const auto &dataSet2 = ((type == Grid::T_LINE) ? summary.getColsByRowNote(i2, vIdx)
                                                           : summary.getRowsByColNote(i2, vIdx));
            if (dataSet == dataSet2)
            {
                int j(-1);
                int changedCount(0);
                while ((j = utils::getNext(dataSet, j)) != -1)
                {
                    if (type == Grid::T_LINE)
                        changedCount +=
                            pGrid.clearColNotes(j, v, [i, i2](int r) { return (r != i) && (r != i2); });
                    else if (type == Grid::T_COLUMN)
                        changedCount +=
                            pGrid.clearRowNotes(j, v, [i, i2](int c) { return (c != i) && (c != i2); });
                }
                if (changedCount > 0)
                    return true;
                else
                    break;
            }
        }
        return false;
    };

    bool changed(false);

    // As processDataFunc will look for a ahead row to match, 'i' may iterate from 0 to 7
    for (int i = 0; i < 8; ++i)
    {
        for (int v = 1; v < 10; ++v)
        {
            // A number was found only 2 times in a row?
            if (const auto &dataSet = summary.getColsByRowNote(i, v - 1); dataSet.count() == 2)
            {
                changed |= processDataFunc(i, v, Grid::T_LINE, dataSet);
            }
            if (const auto &dataSet = summary.getRowsByColNote(i, v - 1); dataSet.count() == 2)
            {
                changed |= processDataFunc(i, v, Grid::T_COLUMN, dataSet);
            }
        }
    }

    return changed;
}

} // namespace sudoku::solver::techniques
