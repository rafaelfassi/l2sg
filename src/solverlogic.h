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

    static void solveCellsWithOnlyOneCandidate(Grid &pGrid, bool *check = nullptr);
    static void solveUniquePossibility(Grid &pGrid, bool *check = nullptr);
    static void solveCandidatesOnlyInBlockLineOrCol(Grid &pGrid);
    static void reduceNotesByFindingChains(Grid &pGrid, int maxChainSize = 7);
    static int solveByGuesses(Grid &pGrid, int maxSolutions = 1);

private:
    int m_level;
};

#endif // SOLVERLOGIC_H
