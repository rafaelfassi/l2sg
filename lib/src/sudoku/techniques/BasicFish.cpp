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

    const auto processSets = [&](int type, int vIdx, int currSize, auto &iCandidatesSet, const auto &jDataFunc) -> bool
    {
        // Make combinations of {currSize} rows
        combination.reset(iCandidatesSet[vIdx].size(), currSize);
        combLst.clear();
        // For each combination of rows
        while (combination.getNextCombination(combLst))
        {
            // Count how many times the value appears for each column in the current combination.
            int jNotesCount[9] = {};
            std::bitset<9> jMergedSet;
            bool good(true);

            for (auto comb : combLst)
            {
                // Get the row number
                const int i = iCandidatesSet[vIdx][comb];
                const auto &jSet = jDataFunc(i, vIdx);
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
                        if (type == Grid::T_LINE)
                        {
                            // Are there notes to be removed?
                            if (summary.getRowsByColNote(j, vIdx).count() > jNotesCount[j])
                            {
                                changedCount += pGrid.clearColNotes(j, vIdx + 1, &log.cellLogs,
                                                                    [vIdx, &combLst, &iCandidatesSet](int r)
                                                                    {
                                                                        for (auto comb : combLst)
                                                                            if (r == iCandidatesSet[vIdx][comb])
                                                                                return false;
                                                                        return true;
                                                                    });
                            }
                        }
                        else if (type == Grid::T_COLUMN)
                        {
                            if (summary.getColsByRowNote(j, vIdx).count() > jNotesCount[j])
                            {
                                changedCount += pGrid.clearRowNotes(j, vIdx + 1, &log.cellLogs,
                                                                    [vIdx, &combLst, &iCandidatesSet](int c)
                                                                    {
                                                                        for (auto comb : combLst)
                                                                            if (c == iCandidatesSet[vIdx][comb])
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
                                    int l, c;
                                    const auto i = iCandidatesSet[vIdx][comb];
                                    pGrid.translateCoordinates(i, j, l, c, type);
                                    if (pGrid.hasNote(l, c, vIdx + 1))
                                        log.cellLogs.emplace_back(l, c, CellAction::RelatedNote, vIdx + 1);
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
        for (int vIdx = 0; vIdx < 9; ++vIdx)
        {
            const int countByRow = summary.getColsByRowNote(i, vIdx).count();
            if ((countByRow > 1) && (countByRow <= currSize))
                candidateRows[vIdx].push_back(i);

            const int countByCol = summary.getRowsByColNote(i, vIdx).count();
            if ((countByCol > 1) && (countByCol <= currSize))
                candidateCols[vIdx].push_back(i);
        }
    }

    for (int vIdx = 0; vIdx < 9; ++vIdx)
    {
        // If there are {currSize} or more rows with the current candidate.
        if (candidateRows[vIdx].size() >= currSize)
        {
            if (processSets(Grid::T_LINE, vIdx, currSize, candidateRows,
                            [&summary](int i, int vIdx) { return summary.getColsByRowNote(i, vIdx); }))
                return true;
        }

        if (candidateCols[vIdx].size() >= currSize)
        {
            if (processSets(Grid::T_COLUMN, vIdx, currSize, candidateCols,
                            [&summary](int i, int vIdx) { return summary.getRowsByColNote(i, vIdx); }))
                return true;
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
