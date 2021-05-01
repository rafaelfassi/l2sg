#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

// Solves Locked Candidates Type 1 (Pointing) and Locked Candidates Type 2 (Claiming)
void lockedCandidates(Grid &pGrid)
{
    // Indexes whether the block's parts of the rows/cols contains the candidates.
    std::bitset<3> blocksInRowSet[9][9]; // [Block0, Block1, Block2] = blocksInRowSet[cand-1, row]
    std::bitset<3> blocksInColSet[9][9]; // [Block0, Block1, Block2] = blocksInColSet[cand-1, col]

    // Fills the data set indexes.
    const auto fillDataFunc = [&](int l, int c, int b) -> bool {
        for (int v = 1; v < 10; ++v)
        {
            if (pGrid.hasNote(l, c, v))
            {
                blocksInRowSet[v - 1][l].set(b % 3, true);
                blocksInColSet[v - 1][c].set(b / 3, true);
            }
        }
        return true;
    };

    const auto processDataFunc = [&pGrid](const int vIdx, const int type, const auto &dataSet) {
        for (int i = 0; i < 9; i += 3)
        {
            // Findings for Type 1 (Pointing)
            std::optional<int> iFoundType1;
            std::bitset<3> foundSetType1;
            std::bitset<3> othersSetType1;

            // Findings for Type 2 (Claiming)
            std::optional<int> iFoundType2;
            std::bitset<3> foundSetType2;
            std::bitset<3> othersSetType2;

            for (int ib = i; ib < i + 3; ++ib)
            {
                const auto blocks = dataSet[vIdx][ib].count();

                // The value must be found in more than one block for the same row.
                if (!iFoundType1.has_value() && (blocks > 1))
                {
                    // Stores the row number and the block's set of the row.
                    iFoundType1 = ib;
                    foundSetType1 = dataSet[vIdx][ib];
                }
                else
                {
                    // Merge the block's set of the other rows.
                    othersSetType1 |= dataSet[vIdx][ib];
                }

                // The value must be found in only one block for the row.
                if (!iFoundType2.has_value() && (blocks == 1))
                {
                    iFoundType2 = ib;
                    foundSetType2 = dataSet[vIdx][ib];
                }
                else
                {
                    othersSetType2 |= dataSet[vIdx][ib];
                }
            }

            // Keep in the found set only the block where the value was found in one line only
            // Example:
            // 0 1 1 Merged set for the other rows
            // 1 0 1 The set of the found row
            // ----- AND operation
            // 0 0 1
            // 1 0 1 The set of the found row
            // ----- XOR operation
            // 1 0 0
            foundSetType1 = (foundSetType1 & othersSetType1) ^ foundSetType1;
            // Now the count of the found set must be 1
            if (iFoundType1.has_value() && foundSetType1.count() == 1)
            {
                // Gets the starting column of the block
                const int j = utils::getFirst(foundSetType1) * 3;
                if (type == Grid::T_LINE)
                {
                    pGrid.clearRowNotes(*iFoundType1, vIdx + 1,
                                        [j](int c) { return (c < j) || (c > j + 2); });
                }
                else if (type == Grid::T_COLUMN)
                {
                    pGrid.clearColNotes(*iFoundType1, vIdx + 1,
                                        [j](int r) { return (r < j) || (r > j + 2); });
                }
            }
            // If Type 1 (Pointing) was not found, let's try Type 2 (Claiming)
            // As the foundSet has only one block, let's use it as a mask to check
            // whether there are candidates to be removed.
            else if (iFoundType2.has_value() && (foundSetType2 & othersSetType2).any())
            {
                const int j = utils::getFirst(foundSetType2) * 3;
                if (type == Grid::T_LINE)
                {
                    const int blk = pGrid.getBlockNumber(*iFoundType2, j);
                    pGrid.clearBlockNotes(blk, vIdx + 1,
                                          [&iFoundType2](int r, int) { return (r != iFoundType2); });
                }
                else if (type == Grid::T_COLUMN)
                {
                    const int blk = pGrid.getBlockNumber(j, *iFoundType2);
                    pGrid.clearBlockNotes(blk, vIdx + 1,
                                          [&iFoundType2](int, int c) { return (c != iFoundType2); });
                }
            }
        }
    };

    pGrid.forAllCells(fillDataFunc);

    for (int vIdx = 0; vIdx < 9; ++vIdx)
    {
        processDataFunc(vIdx, Grid::T_LINE, blocksInRowSet);
        processDataFunc(vIdx, Grid::T_COLUMN, blocksInColSet);
    }
}

} // namespace sudoku::solver::techniques
