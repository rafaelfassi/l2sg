#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "Grid.h"

namespace sudoku
{

enum Level
{
    LEV_0_LOGIC,
    LEV_1_LOGIC,
    LEV_2_LOGIC,
    LEV_3_GUESS,
    LEV_UNKNOWN
};

namespace solver
{

namespace techniques
{

void nakedSingles(Grid &pGrid, bool *check = nullptr);
void hiddenAndNakedSingles(Grid &pGrid, bool *check = nullptr);
void lockedCandidates(Grid &pGrid);
void xWings(Grid &pGrid);
void swordfish(Grid &pGrid);
void hiddenMulti(Grid &pGrid, int maxHidden = 7);
void nakedMulti(Grid &pGrid, int maxNaked = 7);

} // namespace techniques

Level solveLevel(Grid &pGrid, Level maxLevel = LEV_3_GUESS);
int solveByGuesses(Grid &pGrid, int maxSolutions = 1);

} // namespace solver

} // namespace sudoku

#endif // SUDOKU_SOLVER_H
