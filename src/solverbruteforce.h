#ifndef SOLVERBRUTEFORCE_H
#define SOLVERBRUTEFORCE_H

#include <vector>

#include "abstractsolver.h"

class Grid;

class SolverBruteForce : public AbstractSolver
{
public:
    SolverBruteForce(Grid &_pGrid);
    virtual void solve();
    std::vector<Grid> &solveSolutions(size_t _nMaxSolutions = 1);

    void resolve(int lin, int col);

private:
    std::vector<Grid> m_lSolutions;
    size_t m_nMaxSolutions;
};

#endif // SOLVERBRUTEFORCE_H
