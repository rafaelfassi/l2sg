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
    void limparOcorrExterna(QList<int> &cels, int ocorrencias);
    void clearLinNotesExceptEqual(int _nLin, int _nNotes);
    void clearColNotesExceptEqual(int _nCol, int _nNotes);
    void clearBlockNotesExceptEqual(int _nLin, int _nCol, int _nNotes);  
    void getFirstBlockPos(int _nBlockNum, int &_nLin, int &_nCol);

private:
    int m_level;
};

#endif // SOLVERLOGIC_H
