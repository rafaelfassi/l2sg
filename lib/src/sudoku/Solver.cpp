#include "Solver.h"
#include <iostream>
#include <map>
#include <string>

namespace sudoku
{

Level solver::solveLevel(Grid &pGrid, Level maxLevel)
{
    int level(LEV_0_LOGIC);
    std::string notesSignature;
    std::string newNotesSignature;
    bool loop(true);

    do
    {
        if (newNotesSignature.empty())
            notesSignature = pGrid.getNotesSignature();
        else
            notesSignature = newNotesSignature;

        if (level >= LEV_2_LOGIC)
        {
            techniques::hiddenMulti(pGrid);
            techniques::hiddenAndNakedSingles(pGrid);

            techniques::nakedMulti(pGrid);
            techniques::hiddenAndNakedSingles(pGrid);

            techniques::xWings(pGrid);
            techniques::hiddenAndNakedSingles(pGrid);

            techniques::swordfish(pGrid);
            techniques::hiddenAndNakedSingles(pGrid);

            techniques::lockedCandidates(pGrid);
        }
        else if (level >= LEV_1_LOGIC)
        {
            techniques::hiddenMulti(pGrid, 2);
            techniques::hiddenAndNakedSingles(pGrid);

            techniques::lockedCandidates(pGrid);
            techniques::nakedMulti(pGrid, 2);
        }

        techniques::hiddenAndNakedSingles(pGrid);

        if (pGrid.isFull())
            break;

        newNotesSignature = pGrid.getNotesSignature();

        if (newNotesSignature == notesSignature)
        {
            loop = (level < maxLevel);
            if (loop)
            {
                ++level;
            }

            if (level == LEV_3_GUESS)
            {
                loop = false;
                if (solveByGuesses(pGrid) != 1)
                {
                    level = LEV_UNKNOWN;
                }
            }
            else if (!loop)
            {
                level = LEV_UNKNOWN;
            }
        }

    } while (loop);

    return static_cast<Level>(level);
}

int solver::solveByGuesses(Grid &pGrid, int maxSolutions)
{
    using GuessesRank = std::multimap<int, std::pair<int, int>>;
    std::vector<Grid> solutions;
    GuessesRank guessesRank;
    Grid grid = pGrid;

    const std::function<void(GuessesRank::const_iterator)> resolve = [&](GuessesRank::const_iterator it) {
        if (solutions.size() >= maxSolutions)
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
                cell.getNotesLst(candidates);
                for (const int condidate : candidates)
                {
                    Grid savedPoint = grid;
                    grid.setValue(lin, col, condidate);
                    if (grid.clearNotesCascade(lin, col, condidate))
                    {
                        bool noConflicts(true);
                        techniques::nakedSingles(grid, &noConflicts);
                        if (noConflicts)
                        {
                            // This function can also detect the solitary candidates, but calling
                            // nakedSingles first improves a little bit the performance,
                            // as the function is more light weight and may detect a conflict first.
                            techniques::hiddenAndNakedSingles(grid, &noConflicts);
                            if (noConflicts)
                            {
                                if (!grid.hasEmptyNoteForNotSetValue())
                                {
                                    resolve(it);
                                    // This may avoid an extra iteration of the for loop.
                                    if (solutions.size() >= maxSolutions)
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
            if (cell.hasAnyNote())
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

} // namespace sudoku