#include "Grid.h"
#include "CombinationsGen.h"

namespace sudoku::solver::techniques
{

// This function tries to find chains to reduce the number of candidates.
// Being "P" the number of different candidates that can be found in "N" number of cells for a line, column,
// or block, if (P == N) all "P" candidates can be removed from the cells that are not part of "N".
// Example:
//    ......789  ...4..7.9  ...4...89    1..45.78.  1..45.78.  .........    .........  ...4..7.9  .........
//    The numbers 4, 7, 8, 9 are all possible candiadates for the cells 0, 1, 2, 7.
//    P == size(4,7,8,9) == 4
//    N == size(0,1,2,7) == 4
//    The P candidates (4,7,8,9) can be removed from all other cells that are not one of the N !(0,1,2,7).
//    ......789  ...4..7.9  ...4...89    1...5....  1...5....  .........    .........  ...4..7.9  .........
// Source puzzle: 1....8..64...3..81.8............26.32359.61....13......12..536.......7525.726381.
void nakedMulti(Grid &pGrid, int maxNaked)
{
    CombinationsGen combination;
    std::vector<int> combLst;
    combLst.reserve(maxNaked);

    // For each type
    for (int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
    {
        // All the below comments assume (type == T_LINE), but the same logic is applicable
        // for T_COLUMN and T_BLOCK
        // For each row
        for (int i = 0; i < 9; ++i)
        {
            // For each size of combination's chain. (from 2 columns to maxNaked)
            for (int chainSize = 2; (chainSize <= maxNaked); ++chainSize)
            {
                // chainSize possible combinations for 9 cols
                combination.reset(9, chainSize);
                combLst.clear();

                // For each possible combination of columns according to the size of the current chain
                while (combination.getNextCombination(combLst))
                {
                    Cell::Notes cellNotes, totalNotes;
                    std::bitset<9> cells;

                    // Iterate over the current combination of cols and
                    // set all found notes into "totalNotes".
                    // Set into "cells" the columns where the notes were found.
                    for (size_t comb = 0; comb < combLst.size(); ++comb)
                    {
                        const int j = combLst.at(comb);
                        const Cell &cell = pGrid.getTranslatedCell(i, j, type);
                        cellNotes = cell.getNotes();
                        if (!cellNotes.any())
                            break;
                        totalNotes |= cellNotes;
                        cells.set(j);
                    }

                    // The quantity of notes equals to the number of cols they were found?
                    // Notice: If cellNotes is empty, the above loop has break, meaning the col has value.
                    if (cellNotes.any() && (cells.count() == totalNotes.count()))
                    {
                        // For each columns
                        for (int pos = 0; pos < cells.size(); ++pos)
                        {
                            // If the col is not one of the columns of the found chain
                            if (!cells.test(pos))
                            {
                                auto &cell = pGrid.getTranslatedCell(i, pos, type);
                                const auto &notes = cell.getNotes();
                                // The col contains one of the notes that must be removed?
                                if (notes.any() && ((notes & ~totalNotes) != notes))
                                {
                                    // Remove the notes
                                    cell.setNotes(notes & ~totalNotes);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace sudoku::solver::techniques
