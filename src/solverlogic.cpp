#include "solverlogic.h"
#include "combination.h"
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

SolverLogic::SolverLogic(Grid &_pGrid) : AbstractSolver(_pGrid), m_level(LEV_EASY)
{
}

void SolverLogic::solve()
{
    std::string notesSignature;
    std::string newNotesSignature;
    int maxLevel(LEV_GUESS);
    bool loop(true);

    do
    {
        if (newNotesSignature.empty())
            notesSignature = m_pGrid.getNotesSignature();
        else
            notesSignature = newNotesSignature;

        if (m_level >= LEV_HARD)
        {
            solveCandidatesOnlyInBlockLineOrCol(m_pGrid);
            reduceNotesByFindingChains(m_pGrid);
        }
        else if (m_level >= LEV_MEDIUM)
        {
            reduceNotesByFindingChains(m_pGrid, 2);
        }

        // solveSolitaryCandidate(m_pGrid);
        solveUniquePossibility(m_pGrid);

        if (m_pGrid.isFull())
            break;

        newNotesSignature = m_pGrid.getNotesSignature();

        if (newNotesSignature == notesSignature)
        {
            if (m_level < maxLevel)
            {
                ++m_level;
            }

            if (m_level == LEV_GUESS)
            {
                solveByGuess(m_pGrid);
                loop = false;
            }
            else
            {
                loop = (m_level < maxLevel);
            }
        }

    } while (loop);
}

void SolverLogic::solveSolitaryCandidate(Grid &pGrid, bool *check)
{
    bool changed;
    do
    {
        changed = false;

        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                auto &cell = pGrid.getCell(i, j);
                if (cell.notesCount() == 1)
                {
                    std::vector<int> notes;
                    cell.getVisibleNotesLst(notes);
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

void SolverLogic::solveUniquePossibility(Grid &pGrid, bool *check)
{
    int changed;
    int sols[9][9];
    int rows[9][9];
    int cols[9][9];
    std::pair<int, int> blks[9][9];
    std::vector<int> notes;
    constexpr int sz = sizeof(rows) / sizeof(int);

    const std::function<bool(int, int, int)> fillDataFunc = [&](int l, int c, int b) -> bool {
        Cell &cell = pGrid.getCell(l, c);

        notes.clear();
        cell.getVisibleNotesLst(notes);

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
                    case -1:
                        rows[l][noteIdx] = c;
                        break;
                    case -2:
                        break;
                    default:
                        rows[l][noteIdx] = -2;
                }
            }
            {
                switch (cols[c][noteIdx])
                {
                    case -1:
                        cols[c][noteIdx] = l;
                        break;
                    case -2:
                        break;
                    default:
                        cols[c][noteIdx] = -2;
                }
            }
            {
                switch (blks[b][noteIdx].second)
                {
                    case -1:
                        blks[b][noteIdx] = std::make_pair(l, c);
                        break;
                    case -2:
                        break;
                    default:
                        blks[b][noteIdx] = std::make_pair(0, -2);
                }
            }
        }

        return true;
    };

    const auto applyDataFunc = [&pGrid, &check](int l, int c, int v) -> bool {
        auto &cell = pGrid.getCell(l, c);
        if (cell.hasNote())
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
        std::fill(&sols[0][0], &sols[0][0] + sz, -1);
        std::fill(&rows[0][0], &rows[0][0] + sz, -1);
        std::fill(&cols[0][0], &cols[0][0] + sz, -1);
        std::fill(&blks[0][0], &blks[0][0] + sz, std::make_pair(0, -1));
        pGrid.forAllCells(fillDataFunc);

        for (int i = 0; i < 9; ++i)
        {
            for (int v = 1; v < 10; ++v)
            {
                const int vIdx = v - 1;

                if (sols[i][vIdx] > -1)
                {
                    const auto c = sols[i][vIdx];
                    changed |= applyDataFunc(i, c, v);
                    if (check && !(*check))
                        return;
                }

                if (rows[i][vIdx] > -1)
                {
                    const auto c = rows[i][vIdx];
                    changed |= applyDataFunc(i, c, v);
                    if (check && !(*check))
                        return;
                }

                if (cols[i][vIdx] > -1)
                {
                    const auto l = cols[i][vIdx];
                    changed |= applyDataFunc(l, i, v);
                    if (check && !(*check))
                        return;
                }

                if (blks[i][vIdx].second > -1)
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
// 1.....789  12....7.9  .........    12.45.7..  12..5....  .........    ......78.  ...4..78.  ...45....
// .........  .........  1.3......    .........  1....6...  .....67..    .........  ..3...7..  .........
// ......78.  .23...7..  .23....8.    .2.45.7..  .2..5....  .........    .........  .........  ..345....
// The last block has 8 only in the first line, therefore the 8 can be removed from the first line of the
// first block.
// Source puzzle: ..6..3...54.8..2.9.....961.....81...4...9.168.8..345....437....2679483513.....4..
// TODO: The same 8 of (0,0) could also be eliminated by looking that the line 2 has 8 only in the first
// block.
void SolverLogic::solveCandidatesOnlyInBlockLineOrCol(Grid &pGrid)
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
            cell.getVisibleNotesLst(notes);

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

// /*
//     Sendo p o número total de possibilidades diferentes em n células, no caso de p = n podemos remover
//     essas possibilidades das celulas que não pertencem a n. Domínio: (Linha ou Coluna ou Bloco) Ex:
//         XXXX5X78X  XXXXXXXXX  XXXX5X789  XXXXXXXXX  1XX4XXXXX  XXX4X6XXX  XXXX5XX89  1XXX56XXX  1XXX56XXX
//         Neste caso os números 1, 4, 5 e 6 são todas as possibilidades para as células 5, 6, 8 e 9.
//         Então:
//         p = count(1,4,5,6) = 4
//         n = count(5,6,8,9) = 4
//         Podemos remover as possibilidades 1,4,5 e 6 de todas as células diferentes de 5,6,8 e 9.
//         XXXXXX78X  XXXXXXXXX  XXXXXX789  XXXXXXXXX  1XX4XXXXX  XXX4X6XXX  XXXXXXX89  1XXX56XXX  1XXX56XXX
// */

void SolverLogic::reduceNotesByFindingChains(Grid &pGrid, int maxChainSize)
{
    constexpr int n(9);
    Combination combination;
    std::vector<int> combLst;

    if (!maxChainSize)
        maxChainSize = n - 1;
    combLst.reserve(maxChainSize);

    // For each type
    for (int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
    {
        // All the below comments assume (type == T_LINE), but the same logic is applicable
        // for T_COLUMN and T_BLOCK
        // For each row
        for (int i = 0; i < n; ++i)
        {
            bool found(false);
            // For each size of combination's chain. (from 2 columns to maxChainSize)
            for (int chainSize = 2; (!found && (chainSize <= maxChainSize)); ++chainSize)
            {
                combLst.clear();
                combination.reset(n, chainSize);

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
                        if (cellNotes.any())
                        {
                            totalNotes |= cellNotes;
                            cells.set(j);
                        }
                        else
                            break;
                    }

                    // The quantity of notes equals to the number of cols they were found?
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
                                    found = true;
                                }
                            }
                        }
                    }

                    // // The total number of notes is the same as the number of cells in the chain?
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
                    //                     Cell::getVisibleNotesLst(totalNotes, lst);
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

int SolverLogic::solveByGuess(Grid &pGrid, int maxSolution)
{
    using GuessesRank = std::multimap<int, std::pair<int, int>>;
    std::vector<Grid> solutions;
    GuessesRank guessesRank;
    Grid grid = pGrid;

    const std::function<void(GuessesRank::const_iterator)> resolve = [&](GuessesRank::const_iterator it) {
        if (solutions.size() >= maxSolution)
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
                cell.getVisibleNotesLst(candidates);
                for (const int condidate : candidates)
                {
                    Grid savedPoint = grid;
                    grid.setValue(lin, col, condidate);
                    if (grid.clearNotesCascade(lin, col, condidate))
                    {
                        bool noConflicts(true);
                        solveSolitaryCandidate(grid, &noConflicts);
                        if (noConflicts)
                        {
                            // This function can also detect the solitary candidates, but calling
                            // solveSolitaryCandidate first improves a little bit the performance, as the
                            // function is more light weight and may detect a conflict first.
                            solveUniquePossibility(grid, &noConflicts);
                            if (noConflicts)
                            {
                                if (!grid.hasEmptyNoteForNotSetValue())
                                {
                                    resolve(it);
                                    // This may avoid an extra iteration of the for loop.
                                    if (solutions.size() >= maxSolution)
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
            if (cell.hasNote())
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