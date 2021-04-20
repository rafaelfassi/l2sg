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
        LEV_GUESS,
        LEV_BF
    };

    SolverLogic(Grid &_pGrid);
    virtual void solve();
    int getResultLevel() { return m_level; }
    int solveSolitaryCandidate(Grid &pGrid, bool *ok = nullptr);
    int solveUniquePossibility(Grid &pGrid, bool *ok = nullptr);
    void solveCandidatesOnlyInBlockLineOrCol(Grid &pGrid);
    void solveNumCellsEqualNumCandidates(Grid &pGrid, int maxCells = 0);
    int solveBruteForce(int maxSolution = 1);

private:
    int m_level;
};

#endif // SOLVERLOGIC_H
