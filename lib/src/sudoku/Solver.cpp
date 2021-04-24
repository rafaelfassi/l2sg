#include "Solver.h"
#include "CombinationsGen.h"
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Improvement:
// X2XXXXXX9 XXXXXXXXX X2XX567XX   XXXX5XXX9 XXXXXXXXX XXXXXXXXX   X2XX5XXXX XXXXXXXXX XXXXX67XX
// The 6 and 7 can be placed only in two cells, therefore we can cleanup X2XX567XX, keeping only 6 and 7.

namespace sudoku
{

void Solver::resolveCellsWithOnlyOneCandidate(Grid &pGrid, bool *check)
{
    bool changed;
    do
    {
        changed = false;

        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                if (auto &cell = pGrid.getCell(i, j); cell.notesCount() == 1)
                {
                    std::vector<int> notes;
                    cell.getNotesLst(notes);
                    if (check)
                    {
                        if (!pGrid.isAllowedValue(i, j, notes.front()))
                        {
                            *check = false;
                            return;
                        }
                    }
                    cell.setValue(notes.front());
                    if (!pGrid.clearNotesCascade(i, j, notes.front()) && check)
                    {
                        *check = false;
                        return;
                    }
                    changed = true;
                    // std::cout << "Solitary >> Lin: " << i+1 << " - Col: " << j+1 << " - Num: " << x <<
                    // std::endl; pGrid.dump();
                }
            }
        }

    } while (changed);

    if (check)
    {
        *check = true;
    }
}

void Solver::resolveUniquePossibility(Grid &pGrid, bool *check)
{
    constexpr int emptyVal(-1);
    constexpr int duplicatedVal(-2);
    int changed;
    int sols[9][9];                 // col = sols[row,cand-1] - Cells that has only one candidate
    int rows[9][9];                 // col = rows[row,cand-1] - Cells with the unique candidate for a row
    int cols[9][9];                 // row = cols[col,cand-1] - Cells with the unique candidate for a row
    std::pair<int, int> blks[9][9]; // {row,col} = blks[blk,cand-1] - Cells with unique candidate for a block
    std::vector<int> notes;
    constexpr int sz = sizeof(rows) / sizeof(int);

    // Fills the arrays sols, rows, cols, blks for the provided position at once.
    const std::function<bool(int, int, int)> fillDataFunc = [&](int l, int c, int b) -> bool {
        Cell &cell = pGrid.getCell(l, c);

        notes.clear();
        cell.getNotesLst(notes);

        if (notes.size() == 1)
        {
            sols[l][notes.front() - 1] = c;
        }

        for (const auto note : notes)
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

// Solves the case where only one row or column of the block has a specific candidate.
// Example:
//    1.....789  12....7.9  .........    12.45.7..  12..5....  .........    ......78.  ...4..78.  ...45....
//    ........  .........  1.3......    .........  1....6...  .....67..    .........  ..3...7..  .........
//    ......78.  .23...7..  .23....8.    .2.45.7..  .2..5....  .........    .........  .........  ..345....
//    The last block has 8 only in the first line, therefore the 8 can be removed from the first line of the
//    first block.
// Source puzzle: ..6..3...54.8..2.9.....961.....81...4...9.168.8..345....437....2679483513.....4..
// TODO: The same 8 of (0,0) could also be eliminated by looking that the line 2 has 8 only in the first
// block.
void Solver::reduceCandidatesOnlyInBlockLineOrCol(Grid &pGrid)
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
void Solver::reduceCandidatesByFindingChains(Grid &pGrid, int maxChainSize)
{
    CombinationsGen combination;
    std::vector<int> combLst;
    combLst.reserve(maxChainSize);

    // For each type
    for (int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
    {
        // All the below comments assume (type == T_LINE), but the same logic is applicable
        // for T_COLUMN and T_BLOCK
        // For each row
        for (int i = 0; i < 9; ++i)
        {
            // For each size of combination's chain. (from 2 columns to maxChainSize)
            for (int chainSize = 2; (chainSize <= maxChainSize); ++chainSize)
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
                                // The col contains one of the notes that should be removed?
                                if (notes.any() && ((notes & ~totalNotes) != notes))
                                {
                                    // Remove the notes
                                    cell.setNotes(cell.getNotes() & ~totalNotes);
                                }
                            }
                        }
                    }

                    // if (cellNotes.any() && (totalNotes.count() == cells.size()))
                    // {
                    //     // Got a chain!!! :)
                    //     // Limpar todas as ocorrencias contidas em "totalNotes" das celulas que não estajam
                    //     no
                    //     // vetor cells.
                    //     bool first(true);
                    //     for (int j = 0; j < 9; ++j)
                    //     {
                    //         if (cells.find(j) == cells.end())
                    //         {
                    //             int l, c;
                    //             pGrid.translateCoordinates(i, j, l, c, type);
                    //             Cell &cell = pGrid.getCell(l, c);
                    //             Cell::Notes notes = cell.getNotes();
                    //             Cell::Notes newNotes = notes & ~totalNotes;

                    //             if (notes != newNotes)
                    //             {
                    //                 cell.setNotes(newNotes);

                    //                 if (first)
                    //                 {
                    //                     std::cout
                    //                         << "Numero de possibilidades igual ao número de células para "
                    //                         << (type == Grid::T_LINE
                    //                                 ? "Linha"
                    //                                 : (type == Grid::T_COLUMN ? "Coluna" : "Bloco"))
                    //                         << std::endl;
                    //                     std::cout << "Células:" << std::endl;
                    //                     for (const auto j1 : cells)
                    //                     {
                    //                         int l1, c1;
                    //                         pGrid.translateCoordinates(i, j1, l1, c1, type);
                    //                         std::cout << "Lin: " << l1 + 1 << " - Col: " << c1 + 1
                    //                                   << std::endl;
                    //                     }

                    //                     std::cout << "Removendo ocorrencias de ";
                    //                     std::vector<int> lst;
                    //                     Cell::getNotesLst(totalNotes, lst);
                    //                     for (const auto oc : lst)
                    //                     {
                    //                         std::cout << oc << " ";
                    //                     }
                    //                     std::cout << "nas células:" << std::endl;
                    //                 }

                    //                 std::cout << "Lin: " << l + 1 << " - Col: " << c + 1 << std::endl;
                    //                 first = false;
                    //             }
                    //         }
                    //     }

                    //     if (!first)
                    //     {
                    //         // std::cout << std::endl;
                    //         // pGrid.dump();
                    //         return;
                    //     }
                    // }
                }
            }
        }
    }
}

Level Solver::solveLevel(Grid &pGrid, Level maxLevel)
{
    int level(LEV_0_LOGIC);
    std::string notesSignature;
    std::string newNotesSignature;
    bool loop(true);

    do
    {
        if (newNotesSignature.empty())
            notesSignature = pGrid.getNotesSignature();
        else
            notesSignature = newNotesSignature;

        if (level >= LEV_2_LOGIC)
        {
            reduceCandidatesByFindingChains(pGrid);
        }
        else if (level >= LEV_1_LOGIC)
        {
            reduceCandidatesOnlyInBlockLineOrCol(pGrid);
            reduceCandidatesByFindingChains(pGrid, 2);
        }

        resolveUniquePossibility(pGrid);

        if (pGrid.isFull())
            break;

        newNotesSignature = pGrid.getNotesSignature();

        if (newNotesSignature == notesSignature)
        {
            loop = (level < maxLevel);
            if (loop)
            {
                ++level;
            }

            if (level == LEV_3_GUESS)
            {
                loop = false;
                if (solveByGuesses(pGrid) != 1)
                {
                    level = LEV_UNKNOWN;
                }
            }
            else if (!loop)
            {
                level = LEV_UNKNOWN;
            }
        }

    } while (loop);

    return static_cast<Level>(level);
}

int Solver::solveByGuesses(Grid &pGrid, int maxSolutions)
{
    using GuessesRank = std::multimap<int, std::pair<int, int>>;
    std::vector<Grid> solutions;
    GuessesRank guessesRank;
    Grid grid = pGrid;

    const std::function<void(GuessesRank::const_iterator)> resolve = [&](GuessesRank::const_iterator it) {
        if (solutions.size() >= maxSolutions)
            return;

        if (it != guessesRank.end())
        {
            const int lin = it->second.first;
            const int col = it->second.second;
            const auto &cell = grid.getCell(lin, col);
            ++it;
            if (cell.getValue() > 0)
            {
                resolve(it);
            }
            else
            {
                std::vector<int> candidates;
                cell.getNotesLst(candidates);
                for (const int condidate : candidates)
                {
                    Grid savedPoint = grid;
                    grid.setValue(lin, col, condidate);
                    if (grid.clearNotesCascade(lin, col, condidate))
                    {
                        bool noConflicts(true);
                        resolveCellsWithOnlyOneCandidate(grid, &noConflicts);
                        if (noConflicts)
                        {
                            // This function can also detect the solitary candidates, but calling
                            // resolveCellsWithOnlyOneCandidate first improves a little bit the performance,
                            // as the function is more light weight and may detect a conflict first.
                            resolveUniquePossibility(grid, &noConflicts);
                            if (noConflicts)
                            {
                                if (!grid.hasEmptyNoteForNotSetValue())
                                {
                                    resolve(it);
                                    // This may avoid an extra iteration of the for loop.
                                    if (solutions.size() >= maxSolutions)
                                        break;
                                }
                            }
                        }
                    }

                    // When the function hits here, either the guess failed, or the puzzle is done.
                    // If the puzzle is done, the solutions are already placed in the solutions' vector.
                    // If the guess failed, the previous grid must be restored to try another candidate.
                    grid = savedPoint;
                }
            }
        }
        else
        {
            solutions.push_back(grid);
        }
    };

    for (int l = 0; l < 9; ++l)
    {
        for (int c = 0; c < 9; ++c)
        {
            const auto &cell = grid.getCell(l, c);
            if (cell.hasAnyNote())
            {
                // Order by the ones with less candidates
                guessesRank.insert(std::make_pair(cell.notesCount(), std::make_pair(l, c)));
            }
        }
    }

    resolve(guessesRank.begin());

    if (!solutions.empty())
    {
        pGrid = solutions.front();
    }

    return solutions.size();
}

} // namespace sudoku