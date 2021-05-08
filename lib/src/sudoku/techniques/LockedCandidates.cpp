#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

// Solves Locked Candidates Type 1 (Pointing) and Locked Candidates Type 2 (Claiming)
bool lockedCandidates(Grid &pGrid)
{
    // Indexes whether the block's parts of the rows/cols contains the candidates.
    std::bitset<3> blocksInRowSet[9][9]; // [Block0, Block1, Block2] = blocksInRowSet[cand-1, row]
    std::bitset<3> blocksInColSet[9][9]; // [Block0, Block1, Block2] = blocksInColSet[cand-1, col]

    // Fills the data set indexes.
    const auto fillDataFunc = [&](int l, int c, int b, int e) -> bool {
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

    const auto processDataFunc = [&pGrid](const int vIdx, const int type, const auto &dataSet) -> bool {
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

            // Keep in the foundSet only the block where the value was found in one line only
            // Example:
            // 0 1 1 Merged set of the other rows
            // 1 0 1 The set of the found row (if a row was found, it contains two or three blocks)
            // ----- AND operation
            // 0 0 1
            // 1 0 1 The set of the found row
            // ----- XOR operation
            // 1 0 0
            foundSetType1 = (foundSetType1 & othersSetType1) ^ foundSetType1;
            // Now, if foundSet is valid, its count must be 1 (the block with the candidate only in one line)
            if (iFoundType1.has_value() && (foundSetType1.count() == 1))
            {
                // Gets the starting column of the block
                const int j = utils::bitset_it(foundSetType1).front() * 3;
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
                return true;
            }
            // If Type 1 (Pointing) was not found, let's try Type 2 (Claiming)
            // As the foundSet has only one block, it is used as a mask to check
            // whether there are candidates to be removed.
            // Example:
            // 1 0 1 Merged set of the other rows
            // 1 0 0 The set of the found row (if a row was found, it contains only one block)
            // ----- AND operation
            // 1 0 0
            // If the are no candidates to remove, the result is (0 0 0), otherwise it contains only
            // one block for sure. Using any() is more efficient than (count()==1).
            else if (iFoundType2.has_value() && (foundSetType2 & othersSetType2).any())
            {
                // Gets the starting column of the block
                const int j = utils::bitset_it(foundSetType2).front() * 3;
                if (type == Grid::T_LINE)
                {
                    const int blk = pGrid.getBlockNumber(*iFoundType2, j);
                    pGrid.clearBlockNotes(blk, vIdx + 1,
                                          [&iFoundType2](int, int r, int) { return (r != iFoundType2); });
                }
                else if (type == Grid::T_COLUMN)
                {
                    const int blk = pGrid.getBlockNumber(j, *iFoundType2);
                    pGrid.clearBlockNotes(blk, vIdx + 1,
                                          [&iFoundType2](int, int, int c) { return (c != iFoundType2); });
                }
                return true;
            }
        }
        return false;
    };

    pGrid.forAllCells(fillDataFunc);

    bool changed(false);
    for (int vIdx = 0; vIdx < 9; ++vIdx)
    {
        changed |= processDataFunc(vIdx, Grid::T_LINE, blocksInRowSet);
        changed |= processDataFunc(vIdx, Grid::T_COLUMN, blocksInColSet);
    }

    return changed;
}

} // namespace sudoku::solver::techniques
