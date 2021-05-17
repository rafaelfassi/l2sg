#include "Grid.h"
#include "Utils.h"
#include "iostream"

namespace sudoku::solver::techniques
{

bool removeNotesFromDoubleIntersection(Grid &pGrid, int n, const std::vector<std::pair<int, int>> &cells)
{
    Grid::Group rGroup;
    Grid::Group cGroup;
    Grid::Group bGroup;
    Grid::Group brGroup[9];
    Grid::Group bcGroup[9];

    for (const auto &[r, c] : cells)
    {
        const int b = pGrid.getBlockNumber(r, c);
        rGroup.set(r);
        cGroup.set(c);
        bGroup.set(b);
        brGroup[b].set(r);
        bcGroup[b].set(c);
    }

    bool changed(false);
    
    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            std::bitset<3> intersections;

            if (std::find(cells.begin(), cells.end(), std::make_pair(r, c)) != cells.end())
                continue;

            const int b = pGrid.getBlockNumber(r, c);

            if (rGroup.test(r))
            {
                intersections.set(Grid::GT_ROW);
            }

            if (cGroup.test(c))
            {
                intersections.set(Grid::GT_COL);
            }

            if (bGroup.test(b) && !brGroup[b].test(r) && !bcGroup[b].test(c))
            {
                intersections.set(Grid::GT_BLK);
            }

            // if (!bGroup.test(b) || bjGroup[b].test(j))
            //     continue;
            if (intersections.count() > 1 && pGrid.hasNote(r, c, n))
            {
                pGrid.setNote(r, c, n, false);
                changed = true;

                //std::cout << "Removed: " << n << " from: (" << b << ") " << r << "," << c << " count: " << intersections.count() << std::endl;

                // if (logs)
                // {
                //     log.cellLogs.emplace_back(r, c, CellAction::RemovedNote, n);
                // }
            }
        }
    }

    // if (changed && logs)
    // {
    //     for (const auto &cell : cells)
    //     {
    //         int r, c;
    //         pGrid.translateCoordinates(cell.first, cell.second, r, c, gType);
    //         log.cellLogs.emplace_back(r, c, CellAction::RelatedNote, n);
    //     }
    //     logs->push_back(std::move(log));
    // }

    return changed;
}

bool findGroups(Grid &pGrid, int nIdx, int gType, int i, const Grid::Group& jSet)
{
    const auto &summary(pGrid.getSummary());

    auto jIt = utils::bitset_it(jSet).begin();
    const auto jN1 = *jIt;
    const auto jN2 = *(++jIt);

    int rN1, cN1, rN2, cN2;
    pGrid.translateCoordinates(i, jN1, rN1, cN1, gType);
    pGrid.translateCoordinates(i, jN2, rN2, cN2, gType);

    for (int gType1 = Grid::GT_ROW; gType1 <= Grid::GT_BLK; ++gType1)
    {
        if (gType1 == gType)
            continue;
        
        int r1, c1, i1, j1;
        pGrid.translateCoordinates(i, jN1, r1, c1, gType);
        pGrid.translateCoordinates(r1, c1, i1, j1, gType1);

        for (j1 = 0; j1 < 9; ++j1)
        {
            pGrid.translateCoordinates(i1, j1, r1, c1, gType1);
            if ((r1 == rN1 && c1 == cN1) || (r1 == rN2 && c1 == cN2))
                continue;

            const auto& notes1 = pGrid.getNotes(i1, j1, gType1);
            if (notes1.test(nIdx) && notes1.count() == 2)
            {
                for (int gType2 = Grid::GT_ROW; gType2 <= Grid::GT_BLK; ++gType2)
                {
                    if (gType2 == gType)
                        continue;

                    int r2, c2, i2, j2;
                    pGrid.translateCoordinates(i, jN2, r2, c2, gType);
                    pGrid.translateCoordinates(r2, c2, i2, j2, gType2);

                    for (j2 = 0; j2 < 9; ++j2)
                    {
                        pGrid.translateCoordinates(i2, j2, r2, c2, gType2);
                        if ((r2 == rN1 && c2 == cN1) || (r2 == rN2 && c2 == cN2))
                            continue;
                        const auto& notes2 = pGrid.getNotes(i2, j2, gType2);
                        if (notes2 == notes1)
                        {
                            pGrid.translateCoordinates(i1, j1, r1, c1, gType1);
                            pGrid.translateCoordinates(i2, j2, r2, c2, gType2);

                            // std::cout << "Found: " << nIdx + 1 << " Strong link at: (" << rN1 << "," << cN1 << ") (" << rN2 << "," << cN2 << ")"
                            // << " bivalues at: (" << r1 << "," << c1 << ") (" << r2 << "," << c2 << ")" << std::endl;
                            std::vector<std::pair<int, int>> foundPatternCells;
                            foundPatternCells.emplace_back(r1, c1);
                            foundPatternCells.emplace_back(r2, c2);

                            Cell::Notes nRm;
                            nRm.set(nIdx);
                            nRm = (notes1 ^ nRm);

                            const auto n = utils::bitset_it(nRm).front()+1;

                            if (removeNotesFromDoubleIntersection(pGrid, n, foundPatternCells))
                                return true;
                        }

                    }
                }
            }
        }
    }

    return false;
}

bool wWing(Grid &pGrid, Logs *logs)
{
    const auto &summary(pGrid.getSummary());

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        for (int i = 0; i < 9; ++i)
        {
            if (summary.getColsByRowNote(i, nIdx).count() == 2)
            {
                if (findGroups(pGrid, nIdx, Grid::GT_ROW, i, summary.getColsByRowNote(i, nIdx)))
                    return true;
            }
            if (summary.getRowsByColNote(i, nIdx).count() == 2)
            {
                if (findGroups(pGrid, nIdx, Grid::GT_COL, i, summary.getRowsByColNote(i, nIdx)))
                    return true;
            }
            if (summary.getElmsByBlkNote(i, nIdx).count() == 2)
            {
                if (findGroups(pGrid, nIdx, Grid::GT_BLK, i, summary.getElmsByBlkNote(i, nIdx)))
                    return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
