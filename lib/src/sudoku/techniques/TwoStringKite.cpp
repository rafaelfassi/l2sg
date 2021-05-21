#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool twoStringKite(Grid &pGrid, Logs *logs)
{
    ScopedLog log(logs, Technique::TwoStringKite);
    std::vector<std::pair<int, Grid::Group>> rowsBlocksSet[9];
    std::vector<std::pair<int, Grid::Group>> colsBlocksSet[9];
    const auto &summary(pGrid.getSummary());

    // Fills the candidate rows and columns
    for (int i = 0; i < 9; ++i)
    {
        for (int nIdx = 0; nIdx < 9; ++nIdx)
        {
            // The row must have two candidates only
            if (const auto rowSet(summary.getColsByRowNote(i, nIdx)); rowSet.count() == 2)
            {
                Grid::Group blocks;
                for (const auto j : utils::bitset_it(rowSet))
                {
                    blocks.set(Grid::getBlockNumber(i, j));
                }
                // The candidates must be in two different blocks
                if (blocks.count() == 2)
                {
                    rowsBlocksSet[nIdx].emplace_back(i, blocks);
                }
            }
            if (const auto colSet(summary.getRowsByColNote(i, nIdx)); colSet.count() == 2)
            {
                Grid::Group blocks;
                for (const auto j : utils::bitset_it(colSet))
                {
                    blocks.set(Grid::getBlockNumber(j, i));
                }
                if (blocks.count() == 2)
                {
                    colsBlocksSet[nIdx].emplace_back(i, blocks);
                }
            }
        }
    }

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        // For each candidate row
        for (const auto &[r, rowBlocks] : rowsBlocksSet[nIdx])
        {
            // For each block of the row
            for (const auto b : utils::bitset_it(rowBlocks))
            {
                // For each candidate column
                for (const auto &[c, colBlocks] : colsBlocksSet[nIdx])
                {
                    // Check if some of the column blocks matches the block of the row
                    if (colBlocks.test(b))
                    {
                        // Get the columns of the row
                        const auto &colsSet = summary.getColsByRowNote(r, nIdx);
                        // The candidate column must not be in the columns of the row,
                        // otherwise column and row are sharing the same cell.
                        if (colsSet.test(c))
                            continue;

                        // Extracts the candidates that are not in the common block
                        std::vector<std::pair<int, int>> cells;
                        for (const auto co : utils::bitset_it(colsSet))
                        {
                            if (Grid::getBlockNumber(r, co) != b)
                            {
                                cells.emplace_back(r, co);
                                break;
                            }
                        }
                        const auto &rowSet = summary.getRowsByColNote(c, nIdx);
                        for (const auto ro : utils::bitset_it(rowSet))
                        {
                            if (Grid::getBlockNumber(ro, c) != b)
                            {
                                cells.emplace_back(ro, c);
                                break;
                            }
                        }

                        // Tries to remove the candidates that see both of the candidates out of the common block
                        if (pGrid.removeNotesFromIntersections(nIdx + 1, cells, log.getCellsPtr()))
                        {
                            if (log.isEnabled())
                            {
                                for (const auto &cell : cells)
                                {
                                    log.addCellLog(cell, CellAction::InPatternN1, nIdx + 1);
                                }
                                for (const auto c : utils::bitset_it(colsSet))
                                {
                                    if (Grid::getBlockNumber(r, c) == b)
                                    {
                                        log.addCellLog(r, c, CellAction::InPatternN2, nIdx + 1);
                                        break;
                                    }
                                }
                                for (const auto r : utils::bitset_it(rowSet))
                                {
                                    if (Grid::getBlockNumber(r, c) == b)
                                    {
                                        log.addCellLog(r, c, CellAction::InPatternN2, nIdx + 1);
                                        break;
                                    }
                                }
                            }

                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
