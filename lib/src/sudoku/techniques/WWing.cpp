#include "Grid.h"
#include "Utils.h"
#include "iostream"

namespace sudoku::solver::techniques
{

bool removeNotesFromDoubleIntersection(Grid &pGrid, int n, const std::array<std::pair<int, int>, 2> &cells)
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

            if ((intersections.count() > 1) && pGrid.hasNote(r, c, n))
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
    auto jIt = utils::bitset_it(jSet).begin();
    const auto jSL1 = *jIt;
    const auto jSL2 = *(++jIt);

    int rSL1, cSL1, rSL2, cSL2;
    Grid::translateCoordinates(i, jSL1, rSL1, cSL1, gType);
    Grid::translateCoordinates(i, jSL2, rSL2, cSL2, gType);

    for (int gTypeBV1 = Grid::GT_ROW; gTypeBV1 <= Grid::GT_BLK; ++gTypeBV1)
    {
        if (gTypeBV1 == gType)
            continue;
        
        int iBV1, jBV1;
        Grid::translateCoordinates(rSL1, cSL1, iBV1, jBV1, gTypeBV1);
        for (jBV1 = 0; jBV1 < 9; ++jBV1)
        {
            int rBV1, cBV1;
            Grid::translateCoordinates(iBV1, jBV1, rBV1, cBV1, gTypeBV1);
            const auto& notesBV1 = pGrid.getNotes(rBV1, cBV1);
            if (notesBV1.test(nIdx) && (notesBV1.count() == 2))
            {
                if ((rBV1 == rSL1 && cBV1 == cSL1) || (rBV1 == rSL2 && cBV1 == cSL2))
                    continue;

                for (int gTypeBV2 = Grid::GT_ROW; gTypeBV2 <= Grid::GT_BLK; ++gTypeBV2)
                {
                    if (gTypeBV2 == gType)
                        continue;

                    int iBV2, jBV2;
                    Grid::translateCoordinates(rSL2, cSL2, iBV2, jBV2, gTypeBV2);
                    for (jBV2 = 0; jBV2 < 9; ++jBV2)
                    {
                        int rBV2, cBV2;
                        Grid::translateCoordinates(iBV2, jBV2, rBV2, cBV2, gTypeBV2);
                        const auto& notesBV2 = pGrid.getNotes(rBV2, cBV2);
                        if (notesBV2 == notesBV1)
                        {
                            if ((rBV2 == rSL1 && cBV2 == cSL1) || (rBV2 == rSL2 && cBV2 == cSL2) || (rBV2 == rBV1 && cBV2 == cBV1))
                                continue;
                            // std::cout << "Found: " << nIdx + 1 << " Strong link at: (" << rSL1 << "," << cSL1 << ") (" << rSL2 << "," << cSL2 << ")"
                            // << " bivalues at: (" << rBV1 << "," << cBV1 << ") (" << rBV2 << "," << cBV2 << ")" << std::endl;
                            std::array<std::pair<int, int>, 2> foundPatternCells;
                            foundPatternCells[0] = std::make_pair(rBV1, cBV1);
                            foundPatternCells[1] = std::make_pair(rBV2, cBV2);

                            Cell::Notes nRm(notesBV1);
                            nRm.set(nIdx, false);
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
