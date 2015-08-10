#ifndef GRID_H
#define GRID_H

#include "cell.h"

class Grid
{
public:
    Grid();
    Grid(Grid const& other);
    ~Grid();

    enum TranslateType{
        T_LINE,
        T_COLUMN,
        T_BLOCK
    };

    Cell *getCell(int _nLin, int _nCol);
    Cell *getTranslatedCell(int _i, int _j, int type = T_LINE);
    void translateCoordinates(int _i, int _j, int &_l, int &_c, int type = T_LINE);
    int *getMatrixCopy();
    void setValues(int *_pValues);
    int getValue(int _nLin, int _nCol) const;
    void setValue(int _nLin, int _nCol, int _nVal);
    bool getNoteVisible(int _nLin, int _nCol, int _nVal);
    void setNoteVisible(int _nLin, int _nCol, int _nVal, bool _bVisible);
    bool compareValues(int *_pValues);
    bool compareValues(const Grid &_grid);
    void dump();

    bool checkRules(int _nLin, int _nCol, int _nVal);
    bool isFull();
    void fillNotes();
    void clearNotes();
    void clearNotesCascade(int _nLin, int _nCol, int _nValue);
    QString getNotesSignature();

protected:
    Cell *m_cells;
    int m_nSize;
    int m_dumpCount;
};

#endif // GRID_H
