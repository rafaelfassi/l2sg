#include "Grid.h"

#include <unordered_map>
#include <unordered_set>

namespace sudoku::solver::techniques
{

// Solves the case where only one row or column of the block has a specific candidate.
// Example:
//    1.....789  12....7.9  .........    12.45.7..  12..5....  .........    ......78.  ...4..78.  ...45....
//    ........  .........  1.3......    .........  1....6...  .....67..    .........  ..3...7..  .........
//    ......78.  .23...7..  .23....8.    .2.45.7..  .2..5....  .........    .........  .........  ..345....
//    The last block has 8 only in the first line, therefore the 8 can be removed from the first line of the
//    first block.
// Source puzzle: ..6..3...54.8..2.9.....961.....81...4...9.168.8..345....437....2679483513.....4..
// TODO: The same 8 of (0,0) could also be eliminated by looking that the line 2 has 8 only in the first
// block. The Locked Candidates Type 2 must be implemented to solve it:
// http://hodoku.sourceforge.net/en/tech_intersections.php

void lockedCandidates(Grid &pGrid)
{
    std::vector<int> notes;

    // For each block
    for (int i = 0; i < 9; ++i)
    {
        std::unordered_map<int, std::unordered_set<int>> rows;
        std::unordered_map<int, std::unordered_set<int>> cols;

        // For each cell in the block
        for (int j = 0; j < 9; ++j)
        {
            int l, c;
            pGrid.translateCoordinates(i, j, l, c, Grid::T_BLOCK);
            Cell &cell = pGrid.getCell(l, c);
            notes.clear();
            cell.getNotesLst(notes);

            rows[l].insert(notes.begin(), notes.end());
            cols[c].insert(notes.begin(), notes.end());
        }

        for (int val = 1; val <= 9; ++val)
        {
            std::optional<int> target_line;
            std::optional<int> target_col;

            for (const auto &[rowNum, row] : rows)
            {
                if (row.find(val) != row.end())
                {
                    if (!target_line.has_value())
                    {
                        target_line = rowNum;
                    }
                    else
                    {
                        target_line.reset();
                        break;
                    }
                }
            }

            for (const auto &[colNum, col] : cols)
            {
                if (col.find(val) != col.end())
                {
                    if (!target_col.has_value())
                    {
                        target_col = colNum;
                    }
                    else
                    {
                        target_col.reset();
                        break;
                    }
                }
            }

            if (target_line.has_value())
            {
                const auto col = pGrid.getBlockStartCoordinates(i).second;
                pGrid.clearRowNotes(*target_line, val, [col](int c) { return (c < col) || (c > col + 2); });
            }

            if (target_col.has_value())
            {
                const auto row = pGrid.getBlockStartCoordinates(i).first;
                pGrid.clearColNotes(*target_col, val, [row](int r) { return (r < row) || (r > row + 2); });
            }
        }
    }
}

} // namespace sudoku::solver::techniques
