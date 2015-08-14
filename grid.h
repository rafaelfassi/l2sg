#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include <QString>

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

    enum DumpFlags {
        D_ANOTATION = 0x01,
        D_ONE_LINE  = 0x02
    };

    Cell *getCell(int _nLin, int _nCol);
    Cell *getTranslatedCell(int _i, int _j, int type = T_LINE);
    void translateCoordinates(int _i, int _j, int &_l, int &_c, int type = T_LINE);
    int *getMatrixCopy();
    void setValues(int *_pValues);
    void setValues(const QString &values);
    bool fillArrayFormString(const QString &values, int *array);
    int getValue(int _nLin, int _nCol) const;
    void setValue(int _nLin, int _nCol, int _nVal);
    bool getNoteVisible(int _nLin, int _nCol, int _nVal);
    void setNoteVisible(int _nLin, int _nCol, int _nVal, bool _bVisible);
    bool compareValues(int *_pValues);
    bool compareValues(const Grid &_grid);
    void dump(int _dumpFlags = D_ANOTATION, const QString &_null = "0", const QString &_numSep = " ");

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
