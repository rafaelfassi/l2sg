#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool hiddenSingles(Grid &pGrid, Logs *logs)
{
    const auto processDataSet = [&](int nIdx, int i, int gType, const auto &dataSet) {
        int r, c;
        int j = utils::bitset_it(dataSet).front();
        pGrid.translateCoordinates(i, j, r, c, gType);
        pGrid.setValue(r, c, nIdx + 1);
        pGrid.clearNotesCascade(r, c, nIdx + 1);
        if (logs)
        {
            ScopedLog log(logs, Technique::HiddenSingles);
            log.addCellLog(r, c, CellAction::AppliedValue, nIdx + 1);
        }
    };

    const auto& summary(pGrid.getSummary());

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        for (int i = 0; i < 9; ++i)
        {
            if (const auto& dataSet = summary.getColsByRowNote(i, nIdx); dataSet.count() == 1)
            {
                processDataSet(nIdx, i, Grid::GT_ROW, dataSet);
                return true;
            }

            if (const auto& dataSet = summary.getRowsByColNote(i, nIdx); dataSet.count() == 1)
            {
                processDataSet(nIdx, i, Grid::GT_COL, dataSet);
                return true;
            }
            
            if (const auto& dataSet = summary.getElmsByBlkNote(i, nIdx); dataSet.count() == 1)
            {
                processDataSet(nIdx, i, Grid::GT_BLK, dataSet);
                return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
