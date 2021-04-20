#ifndef SOLVERLOGIC_H
#define SOLVERLOGIC_H

#include "abstractsolver.h"

class SolverLogic : public AbstractSolver
{
public:

    enum Levels
    {
        LEV_EASY,
        LEV_MEDIUM,
        LEV_HARD,
        LEV_GUESS
    };

    SolverLogic(Grid &_pGrid);
    virtual void solve();
    int getResultLevel() { return m_level; }

    static void solveSolitaryCandidate(Grid &pGrid, bool *check = nullptr);
    static void solveUniquePossibility(Grid &pGrid, bool *check = nullptr);
    static void solveCandidatesOnlyInBlockLineOrCol(Grid &pGrid);
    static void solveNumCellsEqualNumCandidates(Grid &pGrid, int maxCells = 0);
    static int solveByGuess(Grid &pGrid, int maxSolution = 1);

private:
    int m_level;
};

#endif // SOLVERLOGIC_H
