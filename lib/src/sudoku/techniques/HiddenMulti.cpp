#include "Grid.h"
#include "CombinationsGen.h"

namespace sudoku::solver::techniques
{

bool hiddenMulti(Grid &pGrid, int maxHidden)
{
    CombinationsGen combination;
    std::vector<int> combLst;
    combLst.reserve(maxHidden);

    // For each size of combination's chain. (from 2 values to maxHidden)
    for (int chainSize = 2; (chainSize <= maxHidden); ++chainSize)
    {
        // chainSize possible combinations for 9 values
        combination.reset(9, chainSize);
        combLst.clear();

        // For each possible combination of values according to the size of the current chain
        while (combination.getNextCombination(combLst))
        {
            for (int i = 0; i < 9; ++i)
            {
                for (int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
                {
                    std::bitset<9> cells;
                    Cell::Notes foundNotes;
                    Cell::Notes totalNotes;

                    for (int j = 0; j < 9; ++j)
                    {
                        for (const int v : combLst)
                        {
                            auto &cell = pGrid.getTranslatedCell(i, j, type);
                            if (cell.hasNote(v + 1))
                            {
                                cells.set(j, true);
                                foundNotes.set(v, true);
                                totalNotes |= cell.getNotes();
                            }
                        }
                    }

                    if ((cells.count() == chainSize) && (foundNotes.count() == chainSize) &&
                        (totalNotes.count() > chainSize))
                    {
                        // std::cout << "Found hidden for n " << chainSize << " type: " << type  << " at i: "
                        // << i << " - {"; for (const int v : combLst)
                        // {
                        //     std::cout << v+1 << ",";
                        // }
                        // std::cout << "}" << std::endl;

                        for (int j = 0; j < 9; ++j)
                        {
                            if (cells.test(j))
                            {
                                auto &cell = pGrid.getTranslatedCell(i, j, type);
                                cell.setNotes(cell.getNotes() & foundNotes);
                            }
                        }
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

} // namespace sudoku::solver::techniques
