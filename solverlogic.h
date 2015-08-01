#ifndef SOLVERLOGIC_H
#define SOLVERLOGIC_H

#include "grid.h"

class SolverLogic
{
public:
    SolverLogic(Grid &_pGrid);
    void solve();
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
    Grid &m_pGrid;
};

#endif // SOLVERLOGIC_H
