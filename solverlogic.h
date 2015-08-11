#ifndef SOLVERLOGIC_H
#define SOLVERLOGIC_H

#include "abstractsolver.h"

class SolverLogic : public AbstractSolver
{
public:
    SolverLogic(Grid &_pGrid);
    virtual void solve();
    int getResultLevel() { return m_level; }
    bool existNoteSolitary();
    void noteSolitarySolve();
    void uniquePossibilitySolve();
    void notesReplicatedSolve();
    void numCellQqNumPossibSolve();

private:
    int m_level;
};

#endif // SOLVERLOGIC_H
