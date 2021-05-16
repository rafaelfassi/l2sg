#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool skyscraper(Grid &pGrid, Logs *logs)
{
    Log log(Technique::Skyscraper);
    utils::CombinationsGen combination;
    std::vector<int> combLst;

    const auto removeNotesFromDoubleIntersection = [&](int n, int gType,
                                                       const std::vector<std::pair<int, int>> &cells) -> bool
    {
        Grid::Group iGroup;
        Grid::Group jGroup;
        Grid::Group bGroup;
        Grid::Group bjGroup[9];

        for (const auto &[i, j] : cells)
        {
            const int b = pGrid.getBlockNumber(i, j, gType);
            iGroup.set(i);
            jGroup.set(j);
            bGroup.set(b);
            bjGroup[b].set(j);
        }

        bool changed(false);
        for (const auto j : utils::bitset_it(jGroup))
        {
            for (int i = 0; i < 9; ++i)
            {
                if (iGroup.test(i))
                    continue;
                const int b = pGrid.getBlockNumber(i, j, gType);
                if (!bGroup.test(b) || bjGroup[b].test(j))
                    continue;
                if (pGrid.hasNote(i, j, n, gType))
                {
                    int r, c;
                    pGrid.translateCoordinates(i, j, r, c, gType);
                    pGrid.setNote(r, c, n, false);
                    changed = true;

                    if (logs)
                    {
                        log.cellLogs.emplace_back(r, c, CellAction::RemovedNote, n);
                    }
                }
            }
        }

        if (changed && logs)
        {
            for (const auto &cell : cells)
            {
                int r, c;
                pGrid.translateCoordinates(cell.first, cell.second, r, c, gType);
                log.cellLogs.emplace_back(r, c, CellAction::RelatedNote, n);
            }
            logs->push_back(std::move(log));
        }

        return changed;
    };

    const auto findSkyscraper = [&](int n, int gType, const std::vector<int> &iVec, const auto &jGroupFunc)
    {
        combination.reset(iVec.size(), 2);
        combLst.clear();

        while (combination.getNextCombination(combLst))
        {
            std::array<std::pair<int, Grid::Group>, 2> groups;
            std::bitset<3> bands;
            std::bitset<3> stacks;
            for (auto x = 0; x < 2; ++x)
            {

                const int cmb = combLst[x];
                const int i = iVec[cmb];
                groups[x].first = i;
                groups[x].second |= jGroupFunc(i);
                bands.set(i / 3);

                std::bitset<3> jStacks;
                for (const auto j : utils::bitset_it(groups[x].second))
                {
                    jStacks.set(j / 3);
                }
                if (jStacks.count() != 2)
                {
                    stacks.reset();
                    break;
                }
                stacks |= jStacks;
            }

            std::vector<std::pair<int, int>> foundPatternCells;
            if ((stacks.count() == 2) && (bands.count() == 2) && ((groups[0].second | groups[1].second).count() == 3))
            {
                for (const auto &[i, group] : groups)
                {
                    for (const auto j : utils::bitset_it(group))
                    {
                        foundPatternCells.emplace_back(i, j);
                    }
                }
            }
            if (!foundPatternCells.empty())
            {
                if (removeNotesFromDoubleIntersection(n, gType, foundPatternCells))
                    return true;
            }
        }
        return false;
    };

    const auto &summary(pGrid.getSummary());

    std::vector<int> rowsVec[9];
    std::vector<int> colsVec[9];

    for (int i = 0; i < 9; ++i)
    {
        for (int nIdx = 0; nIdx < 9; ++nIdx)
        {
            if (summary.getColsByRowNote(i, nIdx).count() == 2)
                rowsVec[nIdx].push_back(i);
            if (summary.getRowsByColNote(i, nIdx).count() == 2)
                colsVec[nIdx].push_back(i);
        }
    }

    for (int nIdx = 0; nIdx < 9; ++nIdx)
    {
        if (rowsVec[nIdx].size() > 1)
        {
            if (findSkyscraper(nIdx + 1, Grid::GT_ROW, rowsVec[nIdx],
                               [&summary, nIdx](int i) { return summary.getColsByRowNote(i, nIdx); }))
                return true;
        }

        if (colsVec[nIdx].size() > 1)
        {
            if (findSkyscraper(nIdx + 1, Grid::GT_COL, colsVec[nIdx],
                               [&summary, nIdx](int i) { return summary.getRowsByColNote(i, nIdx); }))
                return true;
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
