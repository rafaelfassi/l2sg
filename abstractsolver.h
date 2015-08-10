#ifndef ABSTRACTSOLVER_H
#define ABSTRACTSOLVER_H

#include "grid.h"

class AbstractSolver
{
public:
    AbstractSolver(Grid &_pGrid);
    virtual void solve() = 0;

protected:
    Grid &m_pGrid;
};

#endif // ABSTRACTSOLVER_H
