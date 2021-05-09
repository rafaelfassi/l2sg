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

    const auto findType1 = [&pGrid](const int vIdx, const int type, const auto &dataSet) -> bool {
        for (int i = 0; i < 9; i += 3)
        {
            int iFound;
            std::bitset<3> foundSet;

            const auto check = [&](int o1, int o2, int o3) -> bool {
                // If the tested row (i+o1) doesn't have candidates in more than one block, there are no
                // candidates to be removed.
                if (dataSet[vIdx][i + o1].count() < 2)
                    return false;

                // Keep in the foundSet the block where the candidate(s) are in one line only (if any)
                // Example:
                // 0 1 1 Merged set of the other rows (i+o2, i+o3)
                // 1 0 1 The set of the row being tested (i+o1)
                // ----- AND operation
                // 0 0 1
                // 1 0 1 The set of the row being tested (i+o1)
                // ----- XOR operation
                // 1 0 0 Result foundSet
                foundSet = (dataSet[vIdx][i + o1] & (dataSet[vIdx][i + o2] | dataSet[vIdx][i + o3])) ^
                           dataSet[vIdx][i + o1];
                // A valid foundSet must have count=1 (the block with candidate(s) only in one row)
                if (foundSet.count() == 1)
                {
                    iFound = i + o1;
                    return true;
                }
                return false;
            };

            if (!check(0, 1, 2))
                if (!check(1, 0, 2))
                    if (!check(2, 1, 0))
                        continue;

            // Gets the starting column of the block
            const int j = utils::bitset_it(foundSet).front() * 3;
            if (type == Grid::T_LINE)
            {
                pGrid.clearRowNotes(iFound, vIdx + 1, [j](int c) { return (c < j) || (c > j + 2); });
            }
            else if (type == Grid::T_COLUMN)
            {
                pGrid.clearColNotes(iFound, vIdx + 1, [j](int r) { return (r < j) || (r > j + 2); });
            }
            return true;
        }
        return false;
    };

    const auto findType2 = [&pGrid](const int vIdx, const int type, const auto &dataSet) -> bool {
        for (int i = 0; i < 9; i += 3)
        {
            int iFound;
            std::bitset<3> foundSet;

            const auto check = [&](int o1, int o2, int o3) -> bool {
                // The tested row (i+o1) must have the candidate(s) in only one block
                if (dataSet[vIdx][i + o1].count() != 1)
                    return false;

                // As the tested row (i+o1) has only one block, it is used as a mask to check
                // whether there are candidates to be removed.
                // Example:
                // 1 0 1 Merged set of the other rows (i+o2, i+o3)
                // 1 0 0 The set of the row being tested (i+o1) (has only one block)
                // ----- AND operation
                // 1 0 0
                // If the are no candidates to remove, the result is {0 0 0}, otherwise it contains only
                // one block for sure. Using any() is more efficient than (count()==1).
                if ((dataSet[vIdx][i + o1] & (dataSet[vIdx][i + o2] | dataSet[vIdx][i + o3])).any())
                {
                    foundSet = dataSet[vIdx][i + o1];
                    iFound = i + o1;
                    return true;
                }
                return false;
            };

            if (!check(0, 1, 2))
                if (!check(1, 0, 2))
                    if (!check(2, 1, 0))
                        continue;

            // Gets the starting column of the block
            const int j = utils::bitset_it(foundSet).front() * 3;
            if (type == Grid::T_LINE)
            {
                const int blk = pGrid.getBlockNumber(iFound, j);
                pGrid.clearBlockNotes(blk, vIdx + 1, [&iFound](int, int r, int) { return (r != iFound); });
            }
            else if (type == Grid::T_COLUMN)
            {
                const int blk = pGrid.getBlockNumber(j, iFound);
                pGrid.clearBlockNotes(blk, vIdx + 1, [&iFound](int, int, int c) { return (c != iFound); });
            }
            return true;
        }
        return false;
    };

    pGrid.forAllCells(fillDataFunc);

    for (int vIdx = 0; vIdx < 9; ++vIdx)
    {
        if (findType1(vIdx, Grid::T_LINE, blocksInRowSet))
            return true;

        if (findType1(vIdx, Grid::T_COLUMN, blocksInColSet))
            return true;

        if (findType2(vIdx, Grid::T_LINE, blocksInRowSet))
            return true;

        if (findType2(vIdx, Grid::T_COLUMN, blocksInColSet))
            return true;
    }

    return false;
}

} // namespace sudoku::solver::techniques
