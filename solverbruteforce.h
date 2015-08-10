#ifndef SOLVERBRUTEFORCE_H
#define SOLVERBRUTEFORCE_H

#include <QList>

#include "abstractsolver.h"

class Grid;

class SolverBruteForce : public AbstractSolver
{
public:
    SolverBruteForce(Grid &_pGrid);
    virtual void solve();
    QList<Grid> &solveSolutions(int _nMaxSolutions = 1);

    void resolve(int lin, int col);

private:
    QList<Grid> m_lSolutions;
    int m_nMaxSolutions;
};

#endif // SOLVERBRUTEFORCE_H
