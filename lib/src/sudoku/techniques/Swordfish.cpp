#include "Grid.h"
#include "Utils.h"

namespace sudoku::solver::techniques
{

bool swordfish(Grid &pGrid, int iniSize, int maxSize)
{
    std::vector<int> combLst;
    utils::CombinationsGen combination;
    const auto &summary(pGrid.getSummary());

    const auto processSets = [&](int type, int vIdx, int currSize, auto &iCandidatesSet) -> bool {
        // Make combinations of {currSize} rows
        combination.reset(iCandidatesSet[vIdx].size(), currSize);
        combLst.clear();
        // For each combination
        while (combination.getNextCombination(combLst))
        {
            // Count how many times the value appears for each column in the current combination.
            int jValueCount[9] = {};
            for (auto comb : combLst)
            {
                // Get the row number
                const int i = iCandidatesSet[vIdx][comb];
                const auto &iSet = ((type == Grid::T_LINE) ? summary.getColsByRowNote(i, vIdx)
                                                           : summary.getRowsByColNote(i, vIdx));

                for (const auto j : utils::bitset_it(iSet))
                {
                    ++jValueCount[j];
                }
            }

            // Check if all rows are fully connected to each other by common columns.
            // For that, the value must appear in at least two rows at the same column, as follow:
            // 1 0 1 0 0
            // 1 0 0 1 0
            // 0 0 1 1 0
            // ---------
            // 2 0 2 2 0
            int connectedCount(0);
            for (int j = 0; j < 9; ++j)
            {
                if (jValueCount[j] == 1)
                {
                    // Column only intersects one row, therefore the rows are not fully connected.
                    connectedCount = 0;
                    break;
                }
                else if (jValueCount[j] > 1)
                {
                    ++connectedCount;
                }
            }

            // Has {currSize} rows fully connected?
            if (connectedCount == currSize)
            {
                int changedCount(false);
                for (int j = 0; j < 9; ++j)
                {
                    if ((jValueCount[j] > 1) && (jValueCount[j] <= currSize))
                    {
                        if (type == Grid::T_LINE)
                        {
                            changedCount +=
                                pGrid.clearColNotes(j, vIdx + 1, [vIdx, &combLst, &iCandidatesSet](int r) {
                                    for (auto comb : combLst)
                                        if (r == iCandidatesSet[vIdx][comb])
                                            return false;
                                    return true;
                                });
                        }
                        else if (type == Grid::T_COLUMN)
                        {
                            changedCount +=
                                pGrid.clearRowNotes(j, vIdx + 1, [vIdx, &combLst, &iCandidatesSet](int c) {
                                    for (auto comb : combLst)
                                        if (c == iCandidatesSet[vIdx][comb])
                                            return false;
                                    return true;
                                });
                        }
                    }
                }
                if (changedCount > 0)
                    return true;
            }
        }
        return false;
    };

    for (int currSize = iniSize; currSize <= maxSize; ++currSize)
    {
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

        bool changed(false);
        for (int vIdx = 0; vIdx < 9; ++vIdx)
        {
            // If there are {currSize} or more rows with the current candidate.
            if (candidateRows[vIdx].size() >= currSize)
            {
                if (processSets(Grid::T_LINE, vIdx, currSize, candidateRows))
                    return true;
            }

            if (candidateCols[vIdx].size() >= currSize)
            {
                if (processSets(Grid::T_COLUMN, vIdx, currSize, candidateCols))
                    return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
