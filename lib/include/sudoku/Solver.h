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
    static void resolveCellsWithOnlyOneCandidate(Grid &pGrid, bool *check = nullptr);
    static void resolveUniquePossibility(Grid &pGrid, bool *check = nullptr);
    static void reduceCandidatesOnlyInBlockLineOrCol(Grid &pGrid);
    static void reduceCandidatesByFindingChains(Grid &pGrid, int maxChainSize = 7);
public:
    static Level solveLevel(Grid &pGrid, Level maxLevel = LEV_3_GUESS);
    static int solveByGuesses(Grid &pGrid, int maxSolutions = 1);
};

} // namespace sudoku

#endif // SUDOKU_SOLVER_H
