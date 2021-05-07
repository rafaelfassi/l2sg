#include "CombinationsGen.h"
#include "Grid.h"

namespace sudoku::solver::techniques
{

bool swordfish(Grid &pGrid)
{
    std::vector<int> combLst;
    CombinationsGen combination;
    const auto &summary(pGrid.getSummary());

    const auto processSets = [&](int type, int v, auto &iCandidatesSet) -> bool {
        const int vIdx = v - 1;

        // If there are three or more rows that are valid candidates (has the same value only 2 or 3 times)
        if (iCandidatesSet[vIdx].size() >= 3)
        {
            // Make combinations of 3 rows
            combination.reset(iCandidatesSet[vIdx].size(), 3);
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
                    for (int j = 0; j < 9; ++j)
                    {
                        if (iSet.test(j))
                        {
                            ++jValueCount[j];
                        }
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

                // Has 3 fully connected rows?
                if (connectedCount == 3)
                {
                    int changedCount(false);
                    for (int j = 0; j < 9; ++j)
                    {
                        if (jValueCount[j] == 2 || jValueCount[j] == 3)
                        {
                            if (type == Grid::T_LINE)
                            {
                                changedCount +=
                                    pGrid.clearColNotes(j, v, [vIdx, &combLst, &iCandidatesSet](int r) {
                                        for (auto comb : combLst)
                                            if (r == iCandidatesSet[vIdx][comb])
                                                return false;
                                        return true;
                                    });
                            }
                            else if (type == Grid::T_COLUMN)
                            {
                                changedCount +=
                                    pGrid.clearRowNotes(j, v, [vIdx, &combLst, &iCandidatesSet](int c) {
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
        }
        return false;
    };

    std::vector<int> candidateRows[9];
    std::vector<int> candidateCols[9];

    // Make sets of candidates rows and cols, where a value appears two or three times.
    for (int i = 0; i < 9; ++i)
    {
        for (int v = 1; v < 10; ++v)
        {
            const int noteIdx = v - 1;

            const int countByRow = summary.getColsByRowNote(i, noteIdx).count();
            if (countByRow == 2 || countByRow == 3)
                candidateRows[noteIdx].push_back(i);

            const int countByCol = summary.getRowsByColNote(i, noteIdx).count();
            if (countByCol == 2 || countByCol == 3)
                candidateCols[noteIdx].push_back(i);
        }
    }

    bool changed(false);
    for (int v = 1; v < 10; ++v)
    {
        changed |= processSets(Grid::T_LINE, v, candidateRows);
        changed |= processSets(Grid::T_COLUMN, v, candidateCols);
    }

    return changed;
}

} // namespace sudoku::solver::techniques
