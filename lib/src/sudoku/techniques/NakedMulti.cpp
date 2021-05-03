#include "CombinationsGen.h"
#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool nakedMulti(Grid &pGrid, int iniMultiplicity, int maxMultiplicity)
{
    CombinationsGen combination;
    std::vector<int> combLst;

    // These buffers are used to improved a little bit the performance, by avoiding calling getTranslatedCell
    // or translateCoordinates inside the loops.
    std::bitset<9> rows[9][9];
    std::bitset<9> cols[9][9];
    std::bitset<9> blks[9][9];

    const auto fillDataFunc = [&](int l, int c, int b, int e) -> bool {
        for (int vIdx = 0; vIdx < 9; ++vIdx)
        {
            if (pGrid.hasNote(l, c, vIdx + 1))
            {
                rows[l][c].set(vIdx, true);
                cols[c][l].set(vIdx, true);
                blks[b][e].set(vIdx, true);
            }
        }
        return true;
    };

    const auto processData = [&pGrid](int i, const auto &jSet, const auto &mergedSet, int type) -> bool {
        bool changed(false);

        int vIdx(-1);
        while ((vIdx = utils::getNext(mergedSet, vIdx)) != -1)
        {
            if (type == Grid::T_LINE)
            {
                changed |= pGrid.clearRowNotes(i, vIdx + 1, [&jSet](int c) { return !jSet.test(c); });
            }
            else if (type == Grid::T_COLUMN)
            {
                changed |= pGrid.clearColNotes(i, vIdx + 1, [&jSet](int r) { return !jSet.test(r); });
            }
            else if (type == Grid::T_BLOCK)
            {
                changed |=
                    pGrid.clearBlockNotes(i, vIdx + 1, [&jSet](int e, int, int) { return !jSet.test(e); });
            }
        }

        return changed;
    };

    pGrid.forAllCells(fillDataFunc);

    combLst.reserve(maxMultiplicity);

    // For each size of combinations.
    for (int multiplicity = iniMultiplicity; (multiplicity <= maxMultiplicity); ++multiplicity)
    {
        // {multiplicity} possible combinations for 9 values
        combination.reset(9, multiplicity);
        combLst.clear();

        // For each possible combination of columns according to the current multiplicity
        while (combination.getNextCombination(combLst))
        {
            // For each row (in fact, it's processing 1 row, 1 col and 1 block at once)
            for (int i = 0; i < 9; ++i)
            {
                std::bitset<9> jSet;
                bool goodRow(true);
                std::bitset<9> mergedRowSet;
                bool goodCol(true);
                std::bitset<9> mergedColSet;
                bool goodBlk(true);
                std::bitset<9> mergedBlkSet;

                // Merge the candidates of the combinations for the row.
                for (const int j : combLst)
                {
                    jSet.set(j, true);

                    if (goodRow)
                    {
                        if (!rows[i][j].any())
                        {
                            goodRow = false;
                        }
                        mergedRowSet |= rows[i][j];
                    }

                    if (goodCol)
                    {
                        if (!cols[i][j].any())
                        {
                            goodCol = false;
                        }
                        mergedColSet |= cols[i][j];
                    }

                    if (goodBlk)
                    {
                        if (!blks[i][j].any())
                        {
                            goodBlk = false;
                        }
                        mergedBlkSet |= blks[i][j];
                    }
                }

                bool changed(false);

                // The total number of candidates equals the number of columns where they were found?
                if (goodRow && (mergedRowSet.count() == multiplicity))
                {
                    changed |= processData(i, jSet, mergedRowSet, Grid::T_LINE);
                }

                if (goodCol && (mergedColSet.count() == multiplicity))
                {
                    changed |= processData(i, jSet, mergedColSet, Grid::T_COLUMN);
                }

                if (goodBlk && (mergedBlkSet.count() == multiplicity))
                {
                    changed |= processData(i, jSet, mergedBlkSet, Grid::T_BLOCK);
                }

                if (changed)
                    return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
