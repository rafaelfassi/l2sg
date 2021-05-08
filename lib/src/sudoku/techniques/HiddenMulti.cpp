#include "Grid.h"
#include "Utils.h"
#include <iostream>

namespace sudoku::solver::techniques
{

bool hiddenMulti(Grid &pGrid, int iniMultiplicity, int maxMultiplicity)
{
    utils::CombinationsGen combination;
    std::vector<int> combLst;
    std::vector<int> validCandVec;

    const auto processCandidates = [&](int i, size_t multiplicity, const auto &availableCandVec, int type,
                                       const auto &sourceDataFunc) -> bool {
        validCandVec.clear();
        for (const auto vIdx : availableCandVec)
        {
            if (sourceDataFunc(i, vIdx).count() <= multiplicity)
            {
                validCandVec.push_back(vIdx);
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
            std::bitset<9> jFoundCandSet;

            // Merge the combinations of the candidates for the row.
            // The result is the columns where the candidates were found.
            for (const int comb : combLst)
            {
                const auto vIdx = validCandVec[comb];
                const auto &jSet = sourceDataFunc(i, vIdx);
                combCandidates.set(vIdx, true);
                jFoundCandSet |= jSet;
            }

            // The number of columns where the candidates were found equals the number of candidates?
            if (jFoundCandSet.count() == multiplicity)
            {
                bool changed(false);
                for (const auto j : utils::bitset_it(jFoundCandSet))
                {
                    auto &cell = pGrid.getTranslatedCell(i, j, type);
                    const auto &notes = cell.getNotes();
                    // Has notes to be removed?
                    if ((notes & combCandidates) != notes)
                    {
                        pGrid.setNotes(i, j, type, Cell::Notes(notes & combCandidates));
                        changed = true;
                        // std::cout << "Found hidden at i: " << i << " type: " << type << std::endl;
                    }
                }
                return changed;
            }
        }
        return false;
    };

    const auto &summary(pGrid.getSummary());

    combLst.reserve(maxMultiplicity);
    validCandVec.reserve(9);

    // For each size of combinations.
    for (int multiplicity = iniMultiplicity; (multiplicity <= maxMultiplicity); ++multiplicity)
    {
        // For each row
        for (int i = 0; i < 9; ++i)
        {
            if (summary.getNotesByRow(i).size() > multiplicity)
            {
                if (processCandidates(
                        i, multiplicity, summary.getNotesByRow(i), Grid::T_LINE,
                        [&summary](int i, int vIdx) { return summary.getColsByRowNote(i, vIdx); }))
                    return true;
            }

            if (summary.getNotesByCol(i).size() > multiplicity)
            {
                if (processCandidates(
                        i, multiplicity, summary.getNotesByCol(i), Grid::T_COLUMN,
                        [&summary](int i, int vIdx) { return summary.getRowsByColNote(i, vIdx); }))
                    return true;
            }

            if (summary.getNotesByBlk(i).size() > multiplicity)
            {
                if (processCandidates(
                        i, multiplicity, summary.getNotesByBlk(i), Grid::T_BLOCK,
                        [&summary](int i, int vIdx) { return summary.getElmsByBlkNote(i, vIdx); }))
                    return true;
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
