#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

bool xWings(Grid &pGrid, Logs *logs)
{
    ScopedLog log(logs, Technique::XWings);
    const auto &summary(pGrid.getSummary());

    const auto processDataFunc = [&](int i, int n, const int gType, const auto &dataSet) -> bool
    {
        const int nIdx = n - 1;
        // Try to find another row in which the same number appers 2 times and in the same columns.
        for (int i2 = i + 1; i2 < 9; ++i2)
        {
            const auto &dataSet2 =
                ((gType == Grid::GT_ROW) ? summary.getColsByRowNote(i2, nIdx) : summary.getRowsByColNote(i2, nIdx));
            if (dataSet == dataSet2)
            {
                int changedCount(0);
                for (const auto j : utils::bitset_it(dataSet))
                {
                    if (gType == Grid::GT_ROW)
                        changedCount += pGrid.clearColNotes(j, n, log.getCellsPtr(),
                                                            [i, i2](int r) { return (r != i) && (r != i2); });
                    else if (gType == Grid::GT_COL)
                        changedCount += pGrid.clearRowNotes(j, n, log.getCellsPtr(),
                                                            [i, i2](int c) { return (c != i) && (c != i2); });
                }
                if (changedCount > 0)
                {
                    if (log.isEnabled())
                    {
                        for (const auto j : utils::bitset_it(dataSet))
                        {
                            int r, c;
                            pGrid.translateCoordinates(i, j, r, c, gType);
                            log.addCellLog(r, c, CellAction::InPatternN1, n);

                            pGrid.translateCoordinates(i2, j, r, c, gType);
                            log.addCellLog(r, c, CellAction::InPatternN1, n);
                        }
                    }
                    return true;
                }

                break;
            }
        }
        return false;
    };

    // As processDataFunc will look for a ahead row to match, 'i' may iterate from 0 to 7
    for (int i = 0; i < 8; ++i)
    {
        for (int n = 1; n < 10; ++n)
        {
            // A number was found only 2 times in a row?
            if (const auto &dataSet = summary.getColsByRowNote(i, n - 1); dataSet.count() == 2)
            {
                if (processDataFunc(i, n, Grid::GT_ROW, dataSet))
                    return true;
            }
            if (const auto &dataSet = summary.getRowsByColNote(i, n - 1); dataSet.count() == 2)
            {
                if (processDataFunc(i, n, Grid::GT_COL, dataSet))
                    return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
