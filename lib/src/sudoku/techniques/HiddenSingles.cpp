#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool hiddenSingles(Grid &pGrid)
{
    const auto processDataSet = [&pGrid](int vIdx, int i, int type, const auto &dataSet) {
        int l, c;
        int j = utils::make_bitset_it(dataSet).front();
        pGrid.translateCoordinates(i, j, l, c, type);
        pGrid.setValue(l, c, vIdx + 1);
        pGrid.clearNotesCascade(l, c, vIdx + 1);
        //std::cout << "hiddenSingles: " << l << "," << c << " = " << vIdx + 1 << std::endl;
    };

    const auto& summary(pGrid.getSummary());

    for (int vIdx = 0; vIdx < 9; ++vIdx)
    {
        for (int i = 0; i < 9; ++i)
        {
            if (const auto& dataSet = summary.getColsByRowNote(i, vIdx); dataSet.count() == 1)
            {
                processDataSet(vIdx, i, Grid::T_LINE, dataSet);
                return true;
            }

            if (const auto& dataSet = summary.getRowsByColNote(i, vIdx); dataSet.count() == 1)
            {
                processDataSet(vIdx, i, Grid::T_COLUMN, dataSet);
                return true;
            }
            
            if (const auto& dataSet = summary.getElmsByBlkNote(i, vIdx); dataSet.count() == 1)
            {
                processDataSet(vIdx, i, Grid::T_BLOCK, dataSet);
                return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
