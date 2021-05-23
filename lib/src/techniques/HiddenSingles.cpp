#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace l2sg::solver::techniques
{

bool hiddenSingles(Grid &pGrid, Logs *logs)
{
    const auto applyCandidate = [&](int n, int i, int j, int gType)
    {
        int r, c;
        pGrid.translateCoordinates(i, j, r, c, gType);
        pGrid.setValue(r, c, n);
        pGrid.clearNotesCascade(r, c, n);
        if (logs)
        {
            ScopedLog log(logs, Technique::HiddenSingles);
            log.addCellLog(r, c, CellAction::AppliedValue, n);
        }
    };

    const auto &summary(pGrid.getSummary());

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        for (int i = 0; i < 9; ++i)
        {
            if (const auto &rowSet = summary.getColsByRowNote(i, nIdx); rowSet.count() == 1)
            {
                applyCandidate(nIdx + 1, i, utils::bitset_it(rowSet).front(), Grid::GT_ROW);
                return true;
            }

            if (const auto &colSet = summary.getRowsByColNote(i, nIdx); colSet.count() == 1)
            {
                applyCandidate(nIdx + 1, i, utils::bitset_it(colSet).front(), Grid::GT_COL);
                return true;
            }

            if (const auto &blkSet = summary.getElmsByBlkNote(i, nIdx); blkSet.count() == 1)
            {
                applyCandidate(nIdx + 1, i, utils::bitset_it(blkSet).front(), Grid::GT_BLK);
                return true;
            }
        }
    }

    return false;
}

} // namespace l2sg::solver::techniques
