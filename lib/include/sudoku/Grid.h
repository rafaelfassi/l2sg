#ifndef GRID_H
#define GRID_H

#include "Cell.h"
#include <functional>
#include <string>

namespace sudoku
{

class Grid
{
public:
    Grid() = default;
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
        D_ANNOTATION = 0x01, // Prints the notes (candidates for the cells)
        D_ONE_LINE = 0x02    // Prints the grid values in one line
    };

    Cell &getCell(int _nLin, int _nCol) { return m_cells[_nLin][_nCol]; }
    Cell &getTranslatedCell(int _i, int _j, int type);
    void translateCoordinates(int _i, int _j, int &_l, int &_c, int type);
    std::pair<int, int> getCellCoordinates(int _cellNum);
    std::pair<int, int> getBlockStartCoordinates(int _b);
    int getBlockNumber(int _l, int _c) const;
    void setValues(int *_pValues);
    void setValues(const std::string &values);
    bool fillArrayFormString(const std::string &values, int *array);
    inline int getValue(int _nLin, int _nCol) const { return m_cells[_nLin][_nCol].getValue(); }
    inline void setValue(int _nLin, int _nCol, int _nVal) { m_cells[_nLin][_nCol].setValue(_nVal); }
    inline bool hasNote(int _nLin, int _nCol, int _nVal) { return m_cells[_nLin][_nCol].hasNote(_nVal); }
    inline void setNote(int _nLin, int _nCol, int _nVal, bool _bActive)
    {
        m_cells[_nLin][_nCol].setNote(_nVal, _bActive);
    }
    bool isAllowedValue(int _nLin, int _nCol, int _nVal);
    bool hasEmptyNoteForNotSetValue();
    bool isFull();
    void fillNotes();
    void clearNotes();
    bool clearNotesCascade(int _nLin, int _nCol, int _nValue);
    void clearRowNotes(int _row, int _val, const std::function<bool(int)> &_clear);
    void clearColNotes(int _col, int _val, const std::function<bool(int)> &_clear);
    std::string getNotesSignature();
    void forAllCells(const std::function<bool(int, int, int)> &_callback);
    bool compareValues(int *_pValues);
    bool compareValues(const Grid &_grid);
    void dump(int _dumpFlags = D_ANNOTATION, const std::string &_null = "0", const std::string &_numSep = " ",
              const std::string &_lineSep = "", const std::string &_colSep = "  ");

protected:
    Cell m_cells[9][9];
};

} // namespace sudoku

#endif // GRID_H
