#include "CombinationsGen.h"
#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool hiddenMulti(Grid &pGrid, int iniMultiplicity, int maxMultiplicity)
{
    CombinationsGen combination;
    std::vector<int> combLst;

    std::bitset<9> rows[9][9];
    std::bitset<9> cols[9][9];
    std::bitset<9> blks[9][9];

    const auto fillDataFunc = [&](int l, int c, int b, int e) -> bool {
        for (int vIdx = 0; vIdx < 9; ++vIdx)
        {
            if (pGrid.hasNote(l, c, vIdx + 1))
            {
                rows[vIdx][l].set(c, true);
                cols[vIdx][c].set(l, true);
                blks[vIdx][b].set(e, true);
            }
        }
        return true;
    };

    const auto processData = [&pGrid](int i, const auto &totalNotes, const auto &mergedSet,
                                      int type) -> bool {
        int j(-1);
        bool changed(false);
        while ((j = utils::getNext(mergedSet, j)) != -1)
        {
            auto &cell = pGrid.getTranslatedCell(i, j, type);
            const auto &notes = cell.getNotes();
            // Has notes to be removed?
            if ((notes & totalNotes) != notes)
            {
                cell.setNotes(notes & totalNotes);
                changed = true;
                // std::cout << "Found hidden at i: " << i << " type: " << type << std::endl;
            }
        }
        return changed;
    };

    combLst.reserve(maxMultiplicity);

    pGrid.forAllCells(fillDataFunc);

    // For each size of combination's chain. (from 2 values to maxMultiplicity)
    for (int multiplicity = iniMultiplicity; (multiplicity <= maxMultiplicity); ++multiplicity)
    {
        // multiplicity possible combinations for 9 values
        combination.reset(9, multiplicity);
        combLst.clear();

        // For each possible combination of candidates according to the current multiplicity
        while (combination.getNextCombination(combLst))
        {
            // For each row
            for (int i = 0; i < 9; ++i)
            {
                Cell::Notes totalNotes;
                bool goodRow(true);
                std::bitset<9> mergedRowSet;
                bool goodCol(true);
                std::bitset<9> mergedColSet;
                bool goodBlk(true);
                std::bitset<9> mergedBlkSet;

                // Merge the combinations of the candidates for the row.
                // The result is the columns where the candidates were found.
                for (const int vIdx : combLst)
                {
                    totalNotes.set(vIdx, true);

                    if (goodRow)
                    {
                        if (!rows[vIdx][i].any())
                        {
                            goodRow = false;
                        }
                        mergedRowSet |= rows[vIdx][i];
                    }

                    if (goodCol)
                    {
                        if (!cols[vIdx][i].any())
                        {
                            goodCol = false;
                        }
                        mergedColSet |= cols[vIdx][i];
                    }

                    if (goodBlk)
                    {
                        if (!blks[vIdx][i].any())
                        {
                            goodBlk = false;
                        }
                        mergedBlkSet |= blks[vIdx][i];
                    }
                }

                bool changed(false);

                // The number of columns where the combinations were found equals the current multiplicity?
                if (goodRow && (mergedRowSet.count() == multiplicity))
                {
                    changed |= processData(i, totalNotes, mergedRowSet, Grid::T_LINE);
                }

                if (goodCol && (mergedColSet.count() == multiplicity))
                {
                    changed |= processData(i, totalNotes, mergedColSet, Grid::T_COLUMN);
                }

                if (goodBlk && (mergedBlkSet.count() == multiplicity))
                {
                    changed |= processData(i, totalNotes, mergedBlkSet, Grid::T_BLOCK);
                }

                if (changed)
                    return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
