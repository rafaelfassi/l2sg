#include "CombinationsGen.h"
#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool nakedMulti(Grid &pGrid, int iniMultiplicity, int maxMultiplicity)
{
    CombinationsGen combination;
    std::vector<int> combLst;
    std::vector<std::pair<int, Cell *>> validCellsVec;
    validCellsVec.reserve(9);
    combLst.reserve(maxMultiplicity);

    const auto processData = [&](int i, int type, int multiplicity) -> bool {
        validCellsVec.clear();
        for (int j = 0; j < 9; ++j)
        {
            auto &cell = pGrid.getTranslatedCell(i, j, type);
            if (!cell.getValue() && (cell.notesCount() <= multiplicity))
            {
                validCellsVec.push_back(std::make_pair(j, &cell));
            }
        }

        if (validCellsVec.size() < multiplicity)
            return false;

        combination.reset(validCellsVec.size(), multiplicity);
        combLst.clear();

        while (combination.getNextCombination(combLst))
        {
            std::bitset<9> jSet;
            std::bitset<9> mergedSet;

            for (const int cmb : combLst)
            {
                const auto &[j, cell] = validCellsVec[cmb];
                jSet.set(j, true);
                mergedSet |= cell->getNotes();
            }

            if (mergedSet.count() == multiplicity)
            {
                bool changed(false);
                int vIdx(-1);
                while ((vIdx = utils::getNext(mergedSet, vIdx)) != -1)
                {
                    if (type == Grid::T_LINE)
                    {
                        changed |= pGrid.clearRowNotes(i, vIdx + 1, [&jSet](int c) { return !jSet.test(c); });
                    }
                    else if (type == Grid::T_COLUMN)
                    {
                        changed |= pGrid.clearColNotes(i, vIdx + 1, [&jSet](int r) { return !jSet.test(r); });
                    }
                    else if (type == Grid::T_BLOCK)
                    {
                        changed |= pGrid.clearBlockNotes(i, vIdx + 1,
                                                         [&jSet](int e, int, int) { return !jSet.test(e); });
                    }
                }

                if (changed)
                    return true;
            }
        }
        return false;
    };

    const auto &summary(pGrid.getSummary());

    // For each size of combinations.
    for (int multiplicity = iniMultiplicity; (multiplicity <= maxMultiplicity); ++multiplicity)
    {
        // For each row
        for (int i = 0; i < 9; ++i)
        {
            if (summary.getNotesByRow(i).size() > multiplicity)
            {
                if (processData(i, Grid::T_LINE, multiplicity))
                    return true;
            }

            if (summary.getNotesByCol(i).size() > multiplicity)
            {
                if (processData(i, Grid::T_COLUMN, multiplicity))
                    return true;
            }

            if (summary.getNotesByBlk(i).size() > multiplicity)
            {
                if (processData(i, Grid::T_BLOCK, multiplicity))
                    return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
