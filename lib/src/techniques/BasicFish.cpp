#include "Grid.h"
#include "Utils.h"

namespace l2sg::solver::techniques
{

bool basicFish(Grid &pGrid, BasicFishType fishType, Logs *logs)
{
    ScopedLog log(logs);
    std::vector<int> combLst;
    utils::CombinationsGen combination;
    const auto &summary(pGrid.getSummary());
    const auto fishSize(static_cast<int>(fishType));

    const auto processSets = [&](int gType, int nIdx, auto &iCandidatesSet, const auto &jDataFunc) -> bool
    {
        // Make combinations of {fishSize} rows
        combination.reset(iCandidatesSet[nIdx].size(), fishSize);
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

                if (jMergedSet.count() > fishSize)
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

            // Has {fishSize} rows fully connected?
            if (connectedCount == fishSize)
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
                                changedCount += pGrid.clearColNotes(j, nIdx + 1, log.getCellsPtr(),
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
                                changedCount += pGrid.clearRowNotes(j, nIdx + 1, log.getCellsPtr(),
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
                    if (log.isEnabled())
                    {
                        switch (fishType)
                        {
                            case BasicFishType::Swordfish:
                                log.setTechnique(Technique::Swordfish);
                                break;
                            case BasicFishType::Jellyfish:
                                log.setTechnique(Technique::Jellyfish);
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
                                        log.addCellLog(r, c, CellAction::InPatternN1, nIdx + 1);
                                }
                            }
                        }
                    }

                    return true;
                }
            }
        }
        return false;
    };

    std::vector<int> candidateRows[9];
    std::vector<int> candidateCols[9];

    // Make sets of candidates rows and cols, where a value appears from two to {fishSize} times.
    for (int i = 0; i < 9; ++i)
    {
        for (int nIdx = 0; nIdx < 9; ++nIdx)
        {
            const int countByRow = summary.getColsByRowNote(i, nIdx).count();
            if ((countByRow > 1) && (countByRow <= fishSize))
                candidateRows[nIdx].push_back(i);

            const int countByCol = summary.getRowsByColNote(i, nIdx).count();
            if ((countByCol > 1) && (countByCol <= fishSize))
                candidateCols[nIdx].push_back(i);
        }
    }

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        // If there are {fishSize} or more rows with the current candidate.
        if (candidateRows[nIdx].size() >= fishSize)
        {
            if (processSets(Grid::GT_ROW, nIdx, candidateRows,
                            [&summary](int i, int nIdx) { return summary.getColsByRowNote(i, nIdx); }))
                return true;
        }

        if (candidateCols[nIdx].size() >= fishSize)
        {
            if (processSets(Grid::GT_COL, nIdx, candidateCols,
                            [&summary](int i, int nIdx) { return summary.getRowsByColNote(i, nIdx); }))
                return true;
        }
    }

    return false;
}

} // namespace l2sg::solver::techniques
