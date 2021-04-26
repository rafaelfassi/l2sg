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

class Solver
{
public:
    static void resolveNakedSingles(Grid &pGrid, bool *check = nullptr);
    static void resolveHiddenAndNakedSingles(Grid &pGrid, bool *check = nullptr);
    static void reduceCandidatesByLockedCandidates(Grid &pGrid);
    static void reduceCandidatesByXWings(Grid &pGrid);
    static void reduceCandidatesBySwordfish(Grid &pGrid);
    static void reduceCandidatesByNakedMultiples(Grid &pGrid, int maxMultiples = 7);

    static Level solveLevel(Grid &pGrid, Level maxLevel = LEV_3_GUESS);
    static int solveByGuesses(Grid &pGrid, int maxSolutions = 1);
};

} // namespace sudoku

#endif // SUDOKU_SOLVER_H
