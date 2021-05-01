#include "Grid.h"

namespace sudoku::solver::techniques
{

void hiddenAndNakedSingles(Grid &pGrid, bool *check)
{
    constexpr int emptyVal(-1);
    constexpr int duplicatedVal(-2);
    int changed;
    int sols[9][9];                 // col = sols[row,cand-1] - Cells that has only one candidate
    int rows[9][9];                 // col = rows[row,cand-1] - Cells with the unique candidate for a row
    int cols[9][9];                 // row = cols[col,cand-1] - Cells with the unique candidate for a row
    std::pair<int, int> blks[9][9]; // {row,col} = blks[blk,cand-1] - Cells with unique candidate for a block
    constexpr int sz = sizeof(rows) / sizeof(int);

    // Fills the arrays sols, rows, cols, blks for the provided position at once.
    const std::function<bool(int, int, int)> fillDataFunc = [&](int l, int c, int b) -> bool {
        Cell &cell = pGrid.getCell(l, c);

        if (cell.notesCount() == 1)
        {
            sols[l][cell.getNextNote(0) - 1] = c;
        }

        int note(0);
        while ((note = cell.getNextNote(note)))
        {
            const int noteIdx = note - 1;
            {
                switch (rows[l][noteIdx])
                {
                    case emptyVal:
                        rows[l][noteIdx] = c;
                        break;
                    case duplicatedVal:
                        break;
                    default:
                        rows[l][noteIdx] = duplicatedVal;
                }
            }
            {
                switch (cols[c][noteIdx])
                {
                    case emptyVal:
                        cols[c][noteIdx] = l;
                        break;
                    case duplicatedVal:
                        break;
                    default:
                        cols[c][noteIdx] = duplicatedVal;
                }
            }
            {
                switch (blks[b][noteIdx].second)
                {
                    case emptyVal:
                        blks[b][noteIdx] = std::make_pair(l, c);
                        break;
                    case duplicatedVal:
                        break;
                    default:
                        blks[b][noteIdx] = std::make_pair(0, duplicatedVal);
                }
            }
        }

        return true;
    };

    // Apply the value to solve the cell
    const auto applyDataFunc = [&pGrid, &check](int l, int c, int v) -> bool {
        auto &cell = pGrid.getCell(l, c);
        // As the unique candidates for row, col and block are being processed at once, the value
        // may be already solved. So that, this check is needed to avoid setting the value again.
        if (cell.hasAnyNote())
        {
            if (check && !pGrid.isAllowedValue(l, c, v))
            {
                *check = false;
                return false;
            }
            cell.setValue(v);
            if (!pGrid.clearNotesCascade(l, c, v) && check)
            {
                *check = false;
                return false;
            }
            return true;
        }
        return false;
    };

    do
    {
        changed = false;
        std::fill(&sols[0][0], &sols[0][0] + sz, emptyVal);
        std::fill(&rows[0][0], &rows[0][0] + sz, emptyVal);
        std::fill(&cols[0][0], &cols[0][0] + sz, emptyVal);
        std::fill(&blks[0][0], &blks[0][0] + sz, std::make_pair(0, emptyVal));
        pGrid.forAllCells(fillDataFunc);

        for (int i = 0; i < 9; ++i)
        {
            for (int v = 1; v < 10; ++v)
            {
                const int vIdx = v - 1;

                if (sols[i][vIdx] > emptyVal)
                {
                    const auto c = sols[i][vIdx];
                    changed |= applyDataFunc(i, c, v);
                    if (check && !(*check))
                        return;
                }

                if (rows[i][vIdx] > emptyVal)
                {
                    const auto c = rows[i][vIdx];
                    changed |= applyDataFunc(i, c, v);
                    if (check && !(*check))
                        return;
                }

                if (cols[i][vIdx] > emptyVal)
                {
                    const auto l = cols[i][vIdx];
                    changed |= applyDataFunc(l, i, v);
                    if (check && !(*check))
                        return;
                }

                if (blks[i][vIdx].second > emptyVal)
                {
                    const auto lc = blks[i][vIdx];
                    changed |= applyDataFunc(lc.first, lc.second, v);
                    if (check && !(*check))
                        return;
                }
            }
        }
    } while (changed);

    if (check)
    {
        *check = true;
    }
}

} // namespace sudoku::solver::techniques
