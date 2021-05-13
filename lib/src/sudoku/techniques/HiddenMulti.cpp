#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool hiddenMulti(Grid &pGrid, HiddenMultiType multiType, Logs *logs)
{
    Log log;
    utils::CombinationsGen combination;
    std::vector<int> combLst;
    std::vector<int> validCandVec;
    const auto multiplicity(static_cast<size_t>(multiType));

    const auto processCandidates = [&](int i, const auto &availableCandVec, int gType, const auto &sourceDataFunc) -> bool
    {
        validCandVec.clear();
        for (const auto nIdx : availableCandVec)
        {
            if (sourceDataFunc(i, nIdx).count() <= multiplicity)
            {
                validCandVec.push_back(nIdx);
            }
        }

        if (validCandVec.size() < multiplicity)
            return false;

        // {multiplicity} possible combinations for 9 values
        combination.reset(validCandVec.size(), multiplicity);
        combLst.clear();

        // For each possible combination of candidates according to the current multiplicity
        while (combination.getNextCombination(combLst))
        {
            // For each row (in fact, it's processing 1 row, 1 col and 1 block at once)
            Cell::Notes combCandidates;
            Grid::Group jFoundCandSet;

            // Merge the combinations of the candidates for the row.
            // The result is the columns where the candidates were found.
            for (const int comb : combLst)
            {
                const auto nIdx = validCandVec[comb];
                const auto &jSet = sourceDataFunc(i, nIdx);
                combCandidates.set(nIdx, true);
                jFoundCandSet |= jSet;
            }

            // The number of columns where the candidates were found equals the number of candidates?
            if (jFoundCandSet.count() == multiplicity)
            {
                bool changed(false);
                for (const auto j : utils::bitset_it(jFoundCandSet))
                {
                    auto &cell = pGrid.getCell(i, j, gType);
                    const auto &notes = cell.getNotes();
                    // Has notes to be removed?
                    if ((notes & combCandidates) != notes)
                    {
                        pGrid.setNotes(i, j, gType, Cell::Notes(notes & combCandidates));
                        changed = true;

                        if (logs)
                        {
                            int r, c;
                            pGrid.translateCoordinates(i, j, r, c, gType);
                            const auto relatedNotes = Cell::Notes(notes & combCandidates);
                            const auto removedNotes = Cell::Notes(relatedNotes ^ notes);
                            for (const auto nIdx : utils::bitset_it(removedNotes))
                            {
                                log.cellLogs.emplace_back(r, c, CellAction::RemovedNote, nIdx + 1);
                            }
                        }
                    }
                }

                if (changed && logs)
                {
                    switch (multiType)
                    {
                    case HiddenMultiType::Pair:
                        log.technique = Technique::HiddenPair;
                        break;
                    case HiddenMultiType::Triple:
                        log.technique = Technique::HiddenTriple;
                        break;
                    case HiddenMultiType::Quadruple:
                        log.technique = Technique::HiddenQuadruple;
                        break;
                    default:
                        break;
                    }

                    for (const auto j : utils::bitset_it(jFoundCandSet))
                    {
                        int r, c;
                        pGrid.translateCoordinates(i, j, r, c, gType);
                        const auto &notes = pGrid.getNotes(r, c);
                        for (const auto nIdx : utils::bitset_it(notes & combCandidates))
                        {
                            log.cellLogs.emplace_back(r, c, CellAction::RelatedNote, nIdx + 1);
                        }
                    }
                    logs->push_back(std::move(log));
                }

                return changed;
            }
        }
        return false;
    };

    const auto &summary(pGrid.getSummary());

    combLst.reserve(multiplicity);
    validCandVec.reserve(9);

    // For each row
    for (int i = 0; i < 9; ++i)
    {
        if (summary.getNotesByRow(i).size() > multiplicity)
        {
            if (processCandidates(i, summary.getNotesByRow(i), Grid::GT_ROW,
                                  [&summary](int i, int nIdx) { return summary.getColsByRowNote(i, nIdx); }))
                return true;
        }

        if (summary.getNotesByCol(i).size() > multiplicity)
        {
            if (processCandidates(i, summary.getNotesByCol(i), Grid::GT_COL,
                                  [&summary](int i, int nIdx) { return summary.getRowsByColNote(i, nIdx); }))
                return true;
        }

        if (summary.getNotesByBlk(i).size() > multiplicity)
        {
            if (processCandidates(i, summary.getNotesByBlk(i), Grid::GT_BLK,
                                  [&summary](int i, int nIdx) { return summary.getElmsByBlkNote(i, nIdx); }))
                return true;
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
