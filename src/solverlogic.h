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
    int solveSolitaryCandidate();
    int solveUniquePossibility();
    void solveCandidatesOnlyInBlockLineOrCol();
    void solveNumCellsEqualNumCandidates(int maxCells = 0);

private:
    int m_level;
};

#endif // SOLVERLOGIC_H
