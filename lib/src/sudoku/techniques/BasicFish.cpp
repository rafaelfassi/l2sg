#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

bool basicFish(Grid &pGrid, BasicFishType fishType, Logs *logs)
{
    Log log;
    std::vector<int> combLst;
    utils::CombinationsGen combination;
    const auto &summary(pGrid.getSummary());

    const auto processSets = [&](int gType, int nIdx, int currSize, auto &iCandidatesSet, const auto &jDataFunc) -> bool
    {
        // Make combinations of {currSize} rows
        combination.reset(iCandidatesSet[nIdx].size(), currSize);
        combLst.clear();
        // For each combination of rows
        while (combination.getNextCombination(combLst))
        {
            // Count how many times the value appears for each column in the current combination.
            int jNotesCount[9] = {};
            Grid::Group jMergedSet;
            bool good(true);

            for (auto comb : combLst)
            {
                // Get the row number
                const int i = iCandidatesSet[nIdx][comb];
                const auto &jSet = jDataFunc(i, nIdx);
                jMergedSet |= jSet;

                if (jMergedSet.count() > currSize)
                {
                    good = false;
                    break;
                }

                for (const auto j : utils::bitset_it(jSet))
                {
                    ++jNotesCount[j];
                }
            }

            if (!good)
                continue;

            // Check if all rows are fully connected to each other by common columns.
            // For that, the note must appear in at least two rows at the same column, as follow:
            // 1 0 1 0 0
            // 1 0 0 1 0
            // 0 0 1 1 0
            // ---------
            // 2 0 2 2 0
            int connectedCount(0);
            for (const auto j : utils::bitset_it(jMergedSet))
            {
                if (jNotesCount[j] == 1)
                {
                    // Column only intersects one row, therefore the rows are not fully connected.
                    connectedCount = 0;
                    break;
                }
                else if (jNotesCount[j] > 1)
                {
                    ++connectedCount;
                }
            }

            // Has {currSize} rows fully connected?
            if (connectedCount == currSize)
            {
                int changedCount(0);
                for (const auto j : utils::bitset_it(jMergedSet))
                {
                    if (jNotesCount[j] > 1)
                    {
                        if (gType == Grid::GT_ROW)
                        {
                            // Are there notes to be removed?
                            if (summary.getRowsByColNote(j, nIdx).count() > jNotesCount[j])
                            {
                                changedCount += pGrid.clearColNotes(j, nIdx + 1, &log.cellLogs,
                                                                    [nIdx, &combLst, &iCandidatesSet](int r)
                                                                    {
                                                                        for (auto comb : combLst)
                                                                            if (r == iCandidatesSet[nIdx][comb])
                                                                                return false;
                                                                        return true;
                                                                    });
                            }
                        }
                        else if (gType == Grid::GT_COL)
                        {
                            if (summary.getColsByRowNote(j, nIdx).count() > jNotesCount[j])
                            {
                                changedCount += pGrid.clearRowNotes(j, nIdx + 1, &log.cellLogs,
                                                                    [nIdx, &combLst, &iCandidatesSet](int c)
                                                                    {
                                                                        for (auto comb : combLst)
                                                                            if (c == iCandidatesSet[nIdx][comb])
                                                                                return false;
                                                                        return true;
                                                                    });
                            }
                        }
                    }
                }

                if (changedCount > 0)
                {
                    if (logs)
                    {
                        switch (currSize)
                        {
                            case static_cast<int>(BasicFishType::Swordfish):
                                log.technique = Technique::Swordfish;
                                break;
                            case static_cast<int>(BasicFishType::Jellyfish):
                                log.technique = Technique::Jellyfish;
                                break;
                            default:
                                break;
                        }

                        for (const auto j : utils::bitset_it(jMergedSet))
                        {
                            if (jNotesCount[j] > 1)
                            {
                                for (auto comb : combLst)
                                {
                                    int r, c;
                                    const auto i = iCandidatesSet[nIdx][comb];
                                    pGrid.translateCoordinates(i, j, r, c, gType);
                                    if (pGrid.hasNote(r, c, nIdx + 1))
                                        log.cellLogs.emplace_back(r, c, CellAction::RelatedNote, nIdx + 1);
                                }
                            }
                        }
                        logs->push_back(std::move(log));
                    }

                    return true;
                }
            }
        }
        return false;
    };

    int currSize = static_cast<int>(fishType);
    std::vector<int> candidateRows[9];
    std::vector<int> candidateCols[9];

    // Make sets of candidates rows and cols, where a value appears from two to {currSize} times.
    for (int i = 0; i < 9; ++i)
    {
        for (int nIdx = 0; nIdx < 9; ++nIdx)
        {
            const int countByRow = summary.getColsByRowNote(i, nIdx).count();
            if ((countByRow > 1) && (countByRow <= currSize))
                candidateRows[nIdx].push_back(i);

            const int countByCol = summary.getRowsByColNote(i, nIdx).count();
            if ((countByCol > 1) && (countByCol <= currSize))
                candidateCols[nIdx].push_back(i);
        }
    }

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        // If there are {currSize} or more rows with the current candidate.
        if (candidateRows[nIdx].size() >= currSize)
        {
            if (processSets(Grid::GT_ROW, nIdx, currSize, candidateRows,
                            [&summary](int i, int nIdx) { return summary.getColsByRowNote(i, nIdx); }))
                return true;
        }

        if (candidateCols[nIdx].size() >= currSize)
        {
            if (processSets(Grid::GT_COL, nIdx, currSize, candidateCols,
                            [&summary](int i, int nIdx) { return summary.getRowsByColNote(i, nIdx); }))
                return true;
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
