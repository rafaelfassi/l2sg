#ifndef SUDOKU_GRID_H
#define SUDOKU_GRID_H

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
        D_VALUES = 0x01,  // Prints the values
        D_NOTES = 0x02,   // Prints the notes (candidates for the cells)
        D_BOARD = 0x04,   // Prints the board (values and notes in one grid)
        D_ONE_LINE = 0x08 // Prints in one line
    };

    Cell &getCell(int _nLin, int _nCol) { return m_cells[_nLin][_nCol]; }
    Cell &getTranslatedCell(int _i, int _j, int type);
    void translateCoordinates(int _i, int _j, int &_l, int &_c, int type);
    std::pair<int, int> getCellCoordinates(int _cellNum);
    std::pair<int, int> getBlockStartCoordinates(int _b);
    int getBlockNumber(int _l, int _c) const;
    void fillValues(int *_pValues);
    bool fillValues(const std::string &values);
    void fillNotes(const std::string &notes);
    void fillBoard(const std::string &board);
    bool fillValuesArrayFormString(const std::string &values, int *array);
    inline int getValue(int _nLin, int _nCol) const { return m_cells[_nLin][_nCol].getValue(); }
    inline void setValue(int _nLin, int _nCol, int _nVal) { m_cells[_nLin][_nCol].setValue(_nVal); }
    inline size_t notesCount(int _nLin, int _nCol) const { return m_cells[_nLin][_nCol].notesCount(); }
    inline void clearNotes(int _nLin, int _nCol) { return m_cells[_nLin][_nCol].clearNotes(); }
    inline Cell::Notes getNotes(int _nLin, int _nCol) const { return m_cells[_nLin][_nCol].getNotes(); }
    inline bool hasNote(int _nLin, int _nCol, int _nVal) { return m_cells[_nLin][_nCol].hasNote(_nVal); }
    inline void setNote(int _nLin, int _nCol, int _nVal, bool _bActive)
    {
        m_cells[_nLin][_nCol].setNote(_nVal, _bActive);
    }
    bool isAllowedValue(int _nLin, int _nCol, int _nVal);
    bool isFull();
    void fillNotes();
    void clearNotes();
    int countNotes(int _nVal, int _i, int _type);
    bool clearNotesCascade(int _nLin, int _nCol, int _nValue);
    int clearRowNotes(int _row, int _val, const std::function<bool(int)> &_clear);
    int clearColNotes(int _col, int _val, const std::function<bool(int)> &_clear);
    int clearBlockNotes(int _blk, int _val, const std::function<bool(int, int, int)> &_clear);
    std::string getNotesSignature();
    void forAllCells(const std::function<bool(int, int, int, int)> &_callback);
    bool compareValues(int *_pValues);
    bool compareValues(const Grid &_grid);
    bool compareNotes(const Grid &_grid);
    void dump(int _dumpFlags = D_VALUES | D_NOTES, const std::string &_empty = ".",
              const std::string &_numSep = "  ", const std::string &_colSep = "    ",
              const std::string &_lineSep = "");

protected:
    Cell m_cells[9][9];
};

} // namespace sudoku

#endif // SUDOKU_GRID_H
