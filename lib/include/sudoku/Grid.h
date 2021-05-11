#ifndef SUDOKU_GRID_H
#define SUDOKU_GRID_H

#include "Cell.h"
#include "Logs.h"
#include <functional>
#include <memory>
#include <string>

namespace sudoku
{

class Grid
{
public:
    const static std::pair<int, int> g_blockElem2RowCol[9][9];

    class Summary
    {
        std::bitset<9> m_colsByRowNote[9][9];
        std::bitset<9> m_rowsByColNote[9][9];
        std::bitset<9> m_elmsByBlkNote[9][9];
        std::vector<int> m_notesByRow[9];
        std::vector<int> m_notesByCol[9];
        std::vector<int> m_notesByBlk[9];

        friend class Grid;
        void updateNote(int l, int c, int vIdx, bool active);
        void updateNotes(int l, int c, const Cell::Notes &oldNotes, const Cell::Notes &newNotes);

    public:
        Summary(const Grid &pGrid);
        Summary(const Summary &other) = delete;
        inline const std::bitset<9> &getColsByRowNote(int l, int vIdx) const
        {
            return m_colsByRowNote[l][vIdx];
        }
        inline const std::bitset<9> &getRowsByColNote(int c, int vIdx) const
        {
            return m_rowsByColNote[c][vIdx];
        }
        inline const std::bitset<9> &getElmsByBlkNote(int b, int vIdx) const
        {
            return m_elmsByBlkNote[b][vIdx];
        }
        inline const std::vector<int> &getNotesByRow(int l) const { return m_notesByRow[l]; }
        inline const std::vector<int> &getNotesByCol(int c) const { return m_notesByCol[c]; }
        inline const std::vector<int> &getNotesByBlk(int b) const { return m_notesByBlk[b]; }
    };

    Grid() = default;
    Grid(const Grid &other) = default;
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

    static inline void translateCoordinates(int _i, int _j, int &_l, int &_c, int type)
    {
        switch (type)
        {
            case T_LINE:
                _l = _i;
                _c = _j;
                break;
            case T_COLUMN:
                _l = _j;
                _c = _i;
                break;
            case T_BLOCK:
                const auto &[l, c] = g_blockElem2RowCol[_i][_j];
                _l = l;
                _c = c;
                break;
        }
    }
    inline Cell &getCell(int _nLin, int _nCol) { return m_cells[_nLin][_nCol]; }
    inline Cell &getTranslatedCell(int _i, int _j, int type)
    {
        int l, c;
        translateCoordinates(_i, _j, l, c, type);
        return m_cells[l][c];
    }
    inline int getBlockNumber(int _l, int _c) const { return g_blockElem2RowCol[_l][_c].first; }
    void fillValues(int *_pValues);
    bool fillValues(const std::string &values);
    void fillNotes(const std::string &notes);
    void fillBoard(const std::string &board);
    bool fillValuesArrayFormString(const std::string &values, int *array);
    inline int getValue(int _nLin, int _nCol) const { return m_cells[_nLin][_nCol].getValue(); }
    inline void setValue(int _nLin, int _nCol, int _nVal) { m_cells[_nLin][_nCol].setValue(_nVal); }
    inline size_t notesCount(int _nLin, int _nCol) const { return m_cells[_nLin][_nCol].notesCount(); }
    inline Cell::Notes getNotes(int _nLin, int _nCol) const { return m_cells[_nLin][_nCol].getNotes(); }
    inline Cell::Notes getTranslatedNotes(int _i, int _j, int type)
    {
        int l, c;
        translateCoordinates(_i, _j, l, c, type);
        return m_cells[l][c].getNotes();
    }
    inline bool hasNote(int _nLin, int _nCol, int _nVal) const
    {
        return m_cells[_nLin][_nCol].hasNote(_nVal);
    }
    inline bool hasNote(int _i, int _j, int _nVal, int type) const
    {
        int l, c;
        translateCoordinates(_i, _j, l, c, type);
        return m_cells[l][c].hasNote(_nVal);
    }
    inline void setNote(int _nLin, int _nCol, int _nVal, bool _bActive)
    {
        m_cells[_nLin][_nCol].setNote(_nVal, _bActive);
        if (m_summary)
            m_summary->updateNote(_nLin, _nCol, _nVal - 1, _bActive);
    }
    void setNotes(int _i, int _j, int _type, const Cell::Notes &_nNotes);
    int clearNotes(int _nLin, int _nCol);
    bool isAllowedValue(int _nLin, int _nCol, int _nVal);
    bool isFull();
    void fillNotes();
    void clearNotes();
    int countNotes(int _nVal, int _i, int _type);
    int clearNotesCascade(int _nLin, int _nCol, int _nValue, CellLogs *cellLogs = nullptr, bool *check = nullptr);
    int clearRowNotes(int _row, int _val, CellLogs *cellLogs = nullptr, const std::function<bool(int)> &_clear = [](int) { return true; });
    int clearColNotes(int _col, int _val, CellLogs *cellLogs = nullptr, const std::function<bool(int)> &_clear = [](int) { return true; });
    int clearBlockNotes(int _blk, int _val, CellLogs *cellLogs = nullptr, const std::function<bool(int, int, int)> &_clear = [](int, int, int) { return true; });
    std::string getNotesSignature();
    void forAllCells(const std::function<bool(int, int, int, int)> &_callback);
    bool compareValues(int *_pValues);
    bool compareValues(const Grid &_grid);
    bool compareNotes(const Grid &_grid);
    void dump(int _dumpFlags = D_VALUES | D_NOTES, const std::string &_empty = ".",
              const std::string &_numSep = "  ", const std::string &_colSep = "    ",
              const std::string &_lineSep = "");

    const Summary &getSummary()
    {
        if (!m_summary)
            m_summary = std::make_shared<Summary>(*this);
        return *m_summary.get();
    }
    void resetSummary() { m_summary.reset(); }

private:
    Cell m_cells[9][9];
    std::shared_ptr<Summary> m_summary;
};

} // namespace sudoku

#endif // SUDOKU_GRID_H
