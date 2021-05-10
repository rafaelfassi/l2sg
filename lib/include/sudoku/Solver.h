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

bool nakedSingles(Grid &pGrid, bool *check = nullptr);
bool hiddenSingles(Grid &pGrid);
bool lockedCandidates(Grid &pGrid);
bool xWings(Grid &pGrid); // basicFish with size=2 does the same job, but this is more efficient.
bool xyWing(Grid &pGrid);
bool basicFish(Grid &pGrid, int iniSize = 3, int maxSize = 4);
bool hiddenMulti(Grid &pGrid, int iniMultiplicity = 2, int maxMultiplicity = 4);
bool nakedMulti(Grid &pGrid, int iniMultiplicity = 2, int maxMultiplicity = 4);

} // namespace techniques

Level solveLevel(Grid &pGrid, Level maxLevel = LEV_3_GUESS);
int solveByGuesses(Grid &pGrid, int maxSolutions = 1);

} // namespace solver

} // namespace sudoku

#endif // SUDOKU_SOLVER_H
