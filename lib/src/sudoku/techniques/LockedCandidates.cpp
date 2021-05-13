#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

// Solves Locked Candidates Type 1 (Pointing) and Locked Candidates Type 2 (Claiming)
bool lockedCandidates(Grid &pGrid, Logs *logs)
{
    // Indexes whether the block's parts of the rows/cols contains the candidates.
    std::bitset<3> blocksInRowSet[9][9]; // [Block0, Block1, Block2] = blocksInRowSet[cand-1, row]
    std::bitset<3> blocksInColSet[9][9]; // [Block0, Block1, Block2] = blocksInColSet[cand-1, col]

    // Fills the data set indexes.
    const auto fillDataFunc = [&](int r, int c, int b, int e) -> bool
    {
        for (int n = 1; n < 10; ++n)
        {
            if (pGrid.hasNote(r, c, n))
            {
                blocksInRowSet[n - 1][r].set(b % 3, true);
                blocksInColSet[n - 1][c].set(b / 3, true);
            }
        }
        return true;
    };

    const auto findType1 = [&pGrid, logs](const int nIdx, const int gType, const auto &dataSet) -> bool
    {
        for (int i = 0; i < 9; i += 3)
        {
            int iFound;
            std::bitset<3> foundSet;

            const auto check = [&](int o1, int o2, int o3) -> bool
            {
                // If the tested row (i+o1) doesn't have candidates in more than one block, there are no
                // candidates to be removed.
                if (dataSet[nIdx][i + o1].count() < 2)
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
                foundSet = (dataSet[nIdx][i + o1] & (dataSet[nIdx][i + o2] | dataSet[nIdx][i + o3])) ^ dataSet[nIdx][i + o1];
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
            const int n = nIdx + 1;
            if (gType == Grid::GT_ROW)
            {
                if (logs)
                {
                    Log log(Technique::LockedCandidatesType1);
                    pGrid.clearRowNotes(iFound, n, &log.cellLogs,
                                        [iFound, j, n, &log](int c)
                                        {
                                            if (!((c < j) || (c > j + 2)))
                                            {
                                                log.cellLogs.emplace_back(iFound, c, CellAction::RelatedNote, n);
                                                return false;
                                            }
                                            return true;
                                        });
                    logs->push_back(std::move(log));
                }
                else
                {
                    pGrid.clearRowNotes(iFound, n, nullptr, [j](int c) { return (c < j) || (c > j + 2); });
                }
            }
            else if (gType == Grid::GT_COL)
            {
                if (logs)
                {
                    Log log(Technique::LockedCandidatesType1);
                    pGrid.clearColNotes(iFound, n, &log.cellLogs,
                                        [iFound, j, n, &log](int r)
                                        {
                                            if (!((r < j) || (r > j + 2)))
                                            {
                                                log.cellLogs.emplace_back(r, iFound, CellAction::RelatedNote, n);
                                                return false;
                                            }
                                            return true;
                                        });
                    logs->push_back(std::move(log));
                }
                else
                {
                    pGrid.clearColNotes(iFound, n, nullptr, [j](int r) { return (r < j) || (r > j + 2); });
                }
            }
            return true;
        }
        return false;
    };

    const auto findType2 = [&pGrid, logs](const int nIdx, const int gType, const auto &dataSet) -> bool
    {
        for (int i = 0; i < 9; i += 3)
        {
            int iFound;
            std::bitset<3> foundSet;

            const auto check = [&](int o1, int o2, int o3) -> bool
            {
                // The tested row (i+o1) must have the candidate(s) in only one block
                if (dataSet[nIdx][i + o1].count() != 1)
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
                if ((dataSet[nIdx][i + o1] & (dataSet[nIdx][i + o2] | dataSet[nIdx][i + o3])).any())
                {
                    foundSet = dataSet[nIdx][i + o1];
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
            const int n = nIdx + 1;
            if (gType == Grid::GT_ROW)
            {
                const int blk = pGrid.getBlockNumber(iFound, j);
                if (logs)
                {
                    Log log(Technique::LockedCandidatesType2);
                    pGrid.clearBlockNotes(blk, n, &log.cellLogs,
                                          [n, &iFound, &log](int, int r, int c)
                                          {
                                              if (r == iFound)
                                              {
                                                  log.cellLogs.emplace_back(r, c, CellAction::RelatedNote, n);
                                                  return false;
                                              }
                                              return true;
                                          });
                    logs->push_back(log);
                }
                else
                {
                    pGrid.clearBlockNotes(blk, n, nullptr, [&iFound](int, int r, int) { return (r != iFound); });
                }
            }
            else if (gType == Grid::GT_COL)
            {
                const int blk = pGrid.getBlockNumber(j, iFound);
                if (logs)
                {
                    Log log(Technique::LockedCandidatesType2);
                    pGrid.clearBlockNotes(blk, n, &log.cellLogs,
                                          [n, &iFound, &log](int, int r, int c)
                                          {
                                              if (c == iFound)
                                              {
                                                  log.cellLogs.emplace_back(r, c, CellAction::RelatedNote, n);
                                                  return false;
                                              }
                                              return true;
                                          });
                    logs->push_back(log);
                }
                else
                {
                    pGrid.clearBlockNotes(blk, n, nullptr, [&iFound](int, int, int c) { return (c != iFound); });
                }
            }
            return true;
        }
        return false;
    };

    pGrid.forAllCells(fillDataFunc);

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        if (findType1(nIdx, Grid::GT_ROW, blocksInRowSet))
            return true;

        if (findType1(nIdx, Grid::GT_COL, blocksInColSet))
            return true;

        if (findType2(nIdx, Grid::GT_ROW, blocksInRowSet))
            return true;

        if (findType2(nIdx, Grid::GT_COL, blocksInColSet))
            return true;
    }

    return false;
}

} // namespace sudoku::solver::techniques
