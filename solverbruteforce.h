#ifndef SOLVERBRUTEFORCE_H
#define SOLVERBRUTEFORCE_H

#include <QList>

#include "grid.h"

class Grid;

class SolverBruteForce
{
public:
    SolverBruteForce(Grid &_pGrid);

    QList<Grid> &solve(int _nMaxSolutions = 1);

    void resolve(int lin, int col);

private:
    Grid &m_pGrid;

    QList<Grid> m_lSolutions;

    int m_nMaxSolutions;
};

#endif // SOLVERBRUTEFORCE_H
