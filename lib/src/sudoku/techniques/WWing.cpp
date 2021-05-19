#include "Grid.h"
#include "Utils.h"
#include "iostream"
#include <unordered_map>

namespace sudoku::solver::techniques
{

bool wWing(Grid &pGrid, Logs *logs)
{
    ScopedLog log(logs, Technique::WWing);

    const auto findWWing = [&](int nIdx, int gType, int i, const Grid::Group &jSet)
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
                const auto notesBV1 = pGrid.getNotes(rBV1, cBV1);
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
                            const auto notesBV2 = pGrid.getNotes(rBV2, cBV2);
                            if (notesBV2 == notesBV1)
                            {
                                if ((rBV2 == rSL1 && cBV2 == cSL1) || (rBV2 == rSL2 && cBV2 == cSL2) ||
                                    (rBV2 == rBV1 && cBV2 == cBV1))
                                    continue;
                                // std::cout << "Found: " << nIdx + 1 << " Strong link at: (" << rSL1 << "," << cSL1 <<
                                // ")
                                // (" << rSL2 << "," << cSL2 << ")"
                                // << " bivalues at: (" << rBV1 << "," << cBV1 << ") (" << rBV2 << "," << cBV2 << ")" <<
                                // std::endl;
                                std::vector<std::pair<int, int>> bVCells;
                                bVCells.emplace_back(rBV1, cBV1);
                                bVCells.emplace_back(rBV2, cBV2);

                                Cell::Notes noteBVRem(notesBV1);
                                noteBVRem.set(nIdx, false);
                                const auto nRm = utils::bitset_it(noteBVRem).front() + 1;

                                bool changed(false);
                                Grid::forIntersections(bVCells,
                                                       [&bVCells, &pGrid, &changed, nRm, &log](int r, int c)
                                                       {
                                                           if (pGrid.hasNote(r, c, nRm) &&
                                                               (std::find(bVCells.begin(), bVCells.end(),
                                                                          std::make_pair(r, c)) == bVCells.end()))
                                                           {
                                                               pGrid.setNote(r, c, nRm, false);
                                                               log.addCellLog(r, c, CellAction::RemovedNote, nRm);
                                                               changed = true;

                                                               // std::cout << r << "," << c << std::endl;
                                                           }
                                                       });

                                if (changed)
                                {
                                    if (log.isEnabled())
                                    {
                                        log.addCellLog(rSL1, cSL1, CellAction::InPatternN2, nIdx + 1);
                                        log.addCellLog(rSL2, cSL2, CellAction::InPatternN2, nIdx + 1);

                                        log.addCellLog(rBV1, cBV1, CellAction::InPatternN2, nIdx + 1);
                                        log.addCellLog(rBV1, cBV1, CellAction::InPatternN1, nRm);

                                        log.addCellLog(rBV2, cBV2, CellAction::InPatternN2, nIdx + 1);
                                        log.addCellLog(rBV2, cBV2, CellAction::InPatternN1, nRm);
                                    }
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    };

    const auto &summary(pGrid.getSummary());

    std::unordered_map<unsigned long, int> bivaluesMap;
    bool bivalues[9] = {};

    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            const auto notes = pGrid.getNotes(r, c);
            if (notes.count() == 2)
            {
                if (++bivaluesMap[notes.to_ulong()] == 2)
                {
                    for (const auto nIdx : utils::bitset_it(notes))
                    {
                        bivalues[nIdx] = true;
                    }
                }
            }
        }
    }

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        if (!bivalues[nIdx])
            continue;

        for (int i = 0; i < 9; ++i)
        {
            if (const auto dataSet = summary.getColsByRowNote(i, nIdx); dataSet.count() == 2)
            {
                if (findWWing(nIdx, Grid::GT_ROW, i, dataSet))
                    return true;
            }
            if (const auto dataSet = summary.getRowsByColNote(i, nIdx); dataSet.count() == 2)
            {
                if (findWWing(nIdx, Grid::GT_COL, i, dataSet))
                    return true;
            }
            if (const auto dataSet = summary.getElmsByBlkNote(i, nIdx); dataSet.count() == 2)
            {
                if (findWWing(nIdx, Grid::GT_BLK, i, dataSet))
                    return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
