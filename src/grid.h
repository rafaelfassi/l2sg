#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include <functional>
#include <string>

class Grid
{
public:
    Grid();
    Grid(Grid const &other) = default;
    ~Grid() = default;

    enum TranslateType
    {
        T_LINE,
        T_COLUMN,
        T_BLOCK
    };

    enum DumpFlags
    {
        D_ANOTATION = 0x01,
        D_ONE_LINE = 0x02
    };

    Cell &getCell(int _nLin, int _nCol);
    Cell &getTranslatedCell(int _i, int _j, int type = T_LINE);
    void translateCoordinates(int _i, int _j, int &_l, int &_c, int type = T_LINE);
    std::pair<int, int> getCellCoordinates(int _cellNum);
    std::pair<int, int> getBlockStartCoordinates(int _b);
    int getBlockNumber(int _l, int _c);
    void setValues(int *_pValues);
    void setValues(const std::string &values);
    bool fillArrayFormString(const std::string &values, int *array);
    int getValue(int _nLin, int _nCol) const;
    void setValue(int _nLin, int _nCol, int _nVal);
    bool getNoteVisible(int _nLin, int _nCol, int _nVal);
    void setNoteVisible(int _nLin, int _nCol, int _nVal, bool _bVisible);
    bool compareValues(int *_pValues);
    bool compareValues(const Grid &_grid);
    void dump(int _dumpFlags = D_ANOTATION, const std::string &_null = "0", const std::string &_numSep = " ",
              const std::string &_lineSep = "", const std::string &_colSep = "  ");

    bool isAllowedValue(int _nLin, int _nCol, int _nVal);
    bool hasEmptyNoteForNotSetValue();
    bool isFull();
    void fillNotes();
    void clearNotes();
    bool clearNotesCascade(int _nLin, int _nCol, int _nValue);
    void clearRowNotes(int _row, int _val, const std::function<bool(int)> &_clear);
    void clearColNotes(int _col, int _val, const std::function<bool(int)> &_clear);
    std::string getNotesSignature();
    void forAllTypes(const std::function<bool(int,int,int)> &_callback);

protected:
    std::array<Cell, 9 * 9> m_cells;
};

#endif // GRID_H
