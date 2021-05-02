#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool hiddenSingles(Grid &pGrid)
{
    std::bitset<9> rows[9][9];
    std::bitset<9> cols[9][9];
    std::bitset<9> blks[9][9];

    const auto fillDataFunc = [&](int l, int c, int b, int e) -> bool {
        for (int vIdx = 0; vIdx < 9; ++vIdx)
        {
            if (pGrid.hasNote(l, c, vIdx + 1))
            {
                rows[vIdx][l].set(c, true);
                cols[vIdx][c].set(l, true);
                blks[vIdx][b].set(e, true);
            }
        }
        return true;
    };

    const auto processDataSet = [&pGrid](int vIdx, int i, int type, const auto &dataSet) -> bool {
        if (dataSet[vIdx][i].count() == 1)
        {
            int l, c;
            int j = utils::getNext(dataSet[vIdx][i], -1);
            pGrid.translateCoordinates(i, j, l, c, type);
            if (pGrid.getValue(l, c) == 0)
            {
                pGrid.setValue(l, c, vIdx + 1);
                pGrid.clearNotesCascade(l, c, vIdx + 1);
                return true;
            }
        }
        return false;
    };

    pGrid.forAllCells(fillDataFunc);

    bool changed(false);

    for (int vIdx = 0; vIdx < 9; ++vIdx)
    {
        for (int i = 0; i < 9; ++i)
        {
            changed |= processDataSet(vIdx, i, Grid::T_LINE, rows);
            changed |= processDataSet(vIdx, i, Grid::T_COLUMN, cols);
            changed |= processDataSet(vIdx, i, Grid::T_BLOCK, blks);
        }
    }

    return changed;
}

} // namespace sudoku::solver::techniques
