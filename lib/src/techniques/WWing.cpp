#include "Grid.h"
#include "Utils.h"
#include "iostream"
#include <unordered_map>

namespace l2sg::solver::techniques
{

bool wWing(Grid &pGrid, Logs *logs)
{
    ScopedLog log(logs, Technique::WWing);

    const auto findWWing = [&](int nIdx, int gType, int i, const Grid::Group &jSet)
    {
        auto jIt = utils::bitset_it(jSet).begin();
        const auto jSL1 = *jIt;
        const auto jSL2 = *(++jIt);

        std::pair<int, int> rcSL1, rcSL2;
        Grid::translateCoordinates(i, jSL1, rcSL1.first, rcSL1.second, gType);
        Grid::translateCoordinates(i, jSL2, rcSL2.first, rcSL2.second, gType);

        for (int gTypeBV1 = Grid::GT_ROW; gTypeBV1 <= Grid::GT_BLK; ++gTypeBV1)
        {
            if (gTypeBV1 == gType)
                continue;

            int iBV1, jBV1;
            Grid::translateCoordinates(rcSL1.first, rcSL1.second, iBV1, jBV1, gTypeBV1);
            for (jBV1 = 0; jBV1 < 9; ++jBV1)
            {
                std::pair<int, int> rcBV1;
                Grid::translateCoordinates(iBV1, jBV1, rcBV1.first, rcBV1.second, gTypeBV1);
                const auto notesBV1 = pGrid.getNotes(rcBV1.first, rcBV1.second);
                if (notesBV1.test(nIdx) && (notesBV1.count() == 2))
                {
                    if ((rcBV1 == rcSL1) || (rcBV1 == rcSL2))
                        continue;

                    for (int gTypeBV2 = Grid::GT_ROW; gTypeBV2 <= Grid::GT_BLK; ++gTypeBV2)
                    {
                        if (gTypeBV2 == gType)
                            continue;

                        int iBV2, jBV2;
                        Grid::translateCoordinates(rcSL2.first, rcSL2.second, iBV2, jBV2, gTypeBV2);
                        for (jBV2 = 0; jBV2 < 9; ++jBV2)
                        {
                            std::pair<int, int> rcBV2;
                            Grid::translateCoordinates(iBV2, jBV2, rcBV2.first, rcBV2.second, gTypeBV2);
                            const auto notesBV2 = pGrid.getNotes(rcBV2.first, rcBV2.second);
                            if (notesBV2 == notesBV1)
                            {
                                if ((rcBV2 == rcSL1) || (rcBV2 == rcSL2) || (rcBV2 == rcBV1))
                                    continue;

                                std::vector<std::pair<int, int>> bVCells;
                                bVCells.emplace_back(rcBV1);
                                bVCells.emplace_back(rcBV2);

                                Cell::Notes noteBVRem(notesBV1);
                                noteBVRem.set(nIdx, false);
                                const auto nRm = utils::bitset_it(noteBVRem).front() + 1;

                                if (pGrid.removeNotesFromIntersections(nRm, bVCells, log.getCellsPtr()))
                                {
                                    if (log.isEnabled())
                                    {
                                        log.addCellLog(rcSL1, CellAction::InPatternN2, nIdx + 1);
                                        log.addCellLog(rcSL2, CellAction::InPatternN2, nIdx + 1);

                                        log.addCellLog(rcBV1, CellAction::InPatternN2, nIdx + 1);
                                        log.addCellLog(rcBV1, CellAction::InPatternN1, nRm);

                                        log.addCellLog(rcBV2, CellAction::InPatternN2, nIdx + 1);
                                        log.addCellLog(rcBV2, CellAction::InPatternN1, nRm);
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

} // namespace l2sg::solver::techniques
