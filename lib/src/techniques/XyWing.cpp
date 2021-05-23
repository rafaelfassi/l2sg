#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace l2sg::solver::techniques
{

bool xyWing(Grid &pGrid, Logs *logs)
{
    std::vector<std::pair<int, int>> foundZVec;

    const auto findZInIntersection = [&](int z, int r1, int c1, int r2, int c2)
    {
        int b1, e1, b2, e2;
        pGrid.translateCoordinates(r1, c1, b1, e1, Grid::GT_BLK);
        pGrid.translateCoordinates(r2, c2, b2, e2, Grid::GT_BLK);

        for (int e = 0; e < 9; ++e)
        {
            int rb, cb;
            pGrid.translateCoordinates(b1, e, rb, cb, Grid::GT_BLK);
            if ((e != e1) && ((rb == r2) || (cb == c2)) && pGrid.hasNote(rb, cb, z))
            {
                foundZVec.push_back(std::make_pair(rb, cb));
            }
            pGrid.translateCoordinates(b2, e, rb, cb, Grid::GT_BLK);
            if ((e != e2) && ((rb == r1) || (cb == c1)) && pGrid.hasNote(rb, cb, z))
            {
                foundZVec.push_back(std::make_pair(rb, cb));
            }
        }
        if (foundZVec.empty())
        {
            if (pGrid.hasNote(r1, c2, z))
                foundZVec.push_back(std::make_pair(r1, c2));
            if (pGrid.hasNote(r2, c1, z))
                foundZVec.push_back(std::make_pair(r2, c1));
        }
    };

    const auto findPincersForPivot = [&](int r, int c) -> bool
    {
        // pivot with X and Y
        const auto pivot = pGrid.getNotes(r, c);

        for (int gTypeP1 = Grid::GT_ROW; gTypeP1 <= Grid::GT_BLK; ++gTypeP1)
        {
            int iP1, jP1Ini;
            pGrid.translateCoordinates(r, c, iP1, jP1Ini, gTypeP1);
            for (int jP1 = 0; jP1 < 9; ++jP1)
            {
                if (jP1 == jP1Ini)
                    continue;
                const auto pincer1 = pGrid.getNotes(iP1, jP1, gTypeP1);
                if (pincer1.count() != 2)
                    continue;
                // The pincer1 contains only X and Z?
                if ((pivot & pincer1).any() && (pivot ^ pincer1).any())
                {
                    // The pincer2 to be found must have only Y and Z
                    const auto pincer2 = (pivot ^ pincer1);
                    for (int gTypeP2 = gTypeP1 + 1; gTypeP2 <= Grid::GT_BLK; ++gTypeP2)
                    {
                        int iP2, jP2Ini;
                        pGrid.translateCoordinates(r, c, iP2, jP2Ini, gTypeP2);
                        for (int jP2 = 0; jP2 < 9; ++jP2)
                        {
                            if (jP2 == jP2Ini)
                                continue;
                            // Found pincer2?
                            if (pincer2 == pGrid.getNotes(iP2, jP2, gTypeP2))
                            {
                                int rP1, cP1, rP2, cP2;
                                pGrid.translateCoordinates(iP1, jP1, rP1, cP1, gTypeP1);
                                pGrid.translateCoordinates(iP2, jP2, rP2, cP2, gTypeP2);
                                const auto b = pGrid.getBlockNumber(r, c);
                                const auto bP1 = pGrid.getBlockNumber(rP1, cP1);
                                const auto bP2 = pGrid.getBlockNumber(rP2, cP2);

                                // Check whether the pivot and the found pincers are not all in the same row,
                                // column or block.
                                if (((r == rP1) && (r == rP2)) || ((c == cP1) && (c == cP2)) ||
                                    ((b == bP1) && (b == bP2)))
                                    continue;

                                // Extracts Z
                                const auto z = utils::bitset_it(pincer1 & pincer2).front() + 1;

                                // Tries to find Z in the intersection of pincer1 and pincer2
                                // Notice: Grid::removeNotesFromIntersections could be used here, but this
                                // specialized function is faster.
                                findZInIntersection(z, rP1, cP1, rP2, cP2);

                                // Removes Z from the cells' notes (if any was found)
                                if (!foundZVec.empty())
                                {
                                    for (const auto &[rRm, cRm] : foundZVec)
                                        pGrid.setNote(rRm, cRm, z, false);

                                    if (logs)
                                    {
                                        ScopedLog log(logs, Technique::XYWing);
                                        for (const auto note : utils::bitset_it(pivot))
                                            log.addCellLog(r, c, CellAction::InPatternN1, note + 1);

                                        for (const auto note : utils::bitset_it(pincer1))
                                        {
                                            CellAction act(note + 1 == z ? CellAction::InPatternN2
                                                                         : CellAction::InPatternN1);
                                            log.addCellLog(rP1, cP1, act, note + 1);
                                        }

                                        for (const auto note : utils::bitset_it(pincer2))
                                        {
                                            CellAction act(note + 1 == z ? CellAction::InPatternN2
                                                                         : CellAction::InPatternN1);
                                            log.addCellLog(rP2, cP2, act, note + 1);
                                        }

                                        for (const auto &[rRm, cRm] : foundZVec)
                                            log.addCellLog(rRm, cRm, CellAction::RemovedNote, z);
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

    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            if (pGrid.getNotes(r, c).count() == 2)
            {
                if (findPincersForPivot(r, c))
                    return true;
            }
        }
    }

    return false;
}

} // namespace l2sg::solver::techniques
