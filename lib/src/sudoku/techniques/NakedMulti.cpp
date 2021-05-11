#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool nakedMulti(Grid &pGrid, NakedMultiType multiType, Logs *logs)
{
    Log log;
    utils::CombinationsGen combination;
    std::vector<int> combLst;
    std::vector<std::pair<int, Cell *>> validCellsVec;
    size_t multiplicity(static_cast<size_t>(multiType));

    validCellsVec.reserve(9);
    combLst.reserve(multiplicity);

    const auto processData = [&](int i, int type) -> bool
    {
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
                int changedCnt(false);
                for (const auto vIdx : utils::bitset_it(mergedSet))
                {
                    if (type == Grid::T_LINE)
                    {
                        changedCnt += pGrid.clearRowNotes(i, vIdx + 1, &log.cellLogs, [&jSet](int c) { return !jSet.test(c); });
                    }
                    else if (type == Grid::T_COLUMN)
                    {
                        changedCnt += pGrid.clearColNotes(i, vIdx + 1, &log.cellLogs, [&jSet](int r) { return !jSet.test(r); });
                    }
                    else if (type == Grid::T_BLOCK)
                    {
                        changedCnt +=
                            pGrid.clearBlockNotes(i, vIdx + 1, &log.cellLogs, [&jSet](int e, int, int) { return !jSet.test(e); });
                    }
                }

                if (changedCnt > 0)
                {
                    if (logs)
                    {
                        switch (multiType)
                        {
                        case NakedMultiType::Pair:
                            log.technique = Technique::NakedPair;
                            break;
                        case NakedMultiType::Triple:
                            log.technique = Technique::NakedTriple;
                            break;
                        case NakedMultiType::Quadruple:
                            log.technique = Technique::NakedQuadruple;
                            break;
                        default:
                            break;
                        }

                        for (const int cmb : combLst)
                        {
                            int l, c;
                            const auto &[j, cell] = validCellsVec[cmb];
                            pGrid.translateCoordinates(i, j, l, c, type);
                            for (const auto vIdx : utils::bitset_it(cell->getNotes()))
                            {
                                log.cellLogs.emplace_back(l, c, CellAction::RelatedNote, vIdx + 1);
                            }
                        }
                        logs->push_back(std::move(log));
                    }
                    return true;
                }
            }
        }
        return false;
    };

    const auto &summary(pGrid.getSummary());

    // For each row
    for (int i = 0; i < 9; ++i)
    {
        if (summary.getNotesByRow(i).size() > multiplicity)
        {
            if (processData(i, Grid::T_LINE))
                return true;
        }

        if (summary.getNotesByCol(i).size() > multiplicity)
        {
            if (processData(i, Grid::T_COLUMN))
                return true;
        }

        if (summary.getNotesByBlk(i).size() > multiplicity)
        {
            if (processData(i, Grid::T_BLOCK))
                return true;
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
