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
    using Group = std::bitset<9>;

    const static std::pair<int, int> g_blockElem2RowCol[9][9];

    class Summary
    {
        Group m_colsByRowNote[9][9];
        Group m_rowsByColNote[9][9];
        Group m_elmsByBlkNote[9][9];
        std::vector<int> m_notesByRow[9];
        std::vector<int> m_notesByCol[9];
        std::vector<int> m_notesByBlk[9];

        friend class Grid;
        void updateNote(int r, int c, int nIdx, bool active);
        void updateNotes(int r, int c, const Cell::Notes &oldNotes, const Cell::Notes &newNotes);

    public:
        Summary(const Grid &pGrid);
        Summary(const Summary &other) = delete;
        inline const Group &getColsByRowNote(int r, int nIdx) const { return m_colsByRowNote[r][nIdx]; }
        inline const Group &getRowsByColNote(int c, int nIdx) const { return m_rowsByColNote[c][nIdx]; }
        inline const Group &getElmsByBlkNote(int b, int nIdx) const { return m_elmsByBlkNote[b][nIdx]; }
        inline const std::vector<int> &getNotesByRow(int r) const { return m_notesByRow[r]; }
        inline const std::vector<int> &getNotesByCol(int c) const { return m_notesByCol[c]; }
        inline const std::vector<int> &getNotesByBlk(int b) const { return m_notesByBlk[b]; }
    };

    Grid() = default;
    Grid(const Grid &other) = default;
    ~Grid() = default;

    enum GroupType
    {
        GT_ROW, // Rows of the grid
        GT_COL, // Columns of the grid
        GT_BLK  // Blocks of the grid
    };

    enum DumpFlags
    {
        D_VALUES = 0x01,  // Prints the values
        D_NOTES = 0x02,   // Prints the notes (candidates for the cells)
        D_BOARD = 0x04,   // Prints the board (values and notes in one grid)
        D_ONE_LINE = 0x08 // Prints in one line
    };

    static inline void translateCoordinates(int _i, int _j, int &_r, int &_c, int gType)
    {
        switch (gType)
        {
            case GT_ROW:
                _r = _i;
                _c = _j;
                break;
            case GT_COL:
                _r = _j;
                _c = _i;
                break;
            case GT_BLK:
                const auto &[r, c] = g_blockElem2RowCol[_i][_j];
                _r = r;
                _c = c;
                break;
        }
    }
    static inline int getBlockNumber(int _r, int _c) { return g_blockElem2RowCol[_r][_c].first; }

    inline Cell &getCell(int _row, int _col) { return m_cells[_row][_col]; }
    inline Cell &getCell(int _i, int _j, int gType)
    {
        int r, c;
        translateCoordinates(_i, _j, r, c, gType);
        return m_cells[r][c];
    }
    inline int getBlockNumber(int _i, int _j, int gType) const
    {
        int r, c;
        translateCoordinates(_i, _j, r, c, gType);
        return g_blockElem2RowCol[r][c].first;
    }
    void fillValues(int *_pValues);
    bool fillValues(const std::string &values);
    void fillNotes(const std::string &notes);
    void fillBoard(const std::string &board);
    bool fillValuesArrayFormString(const std::string &values, int *array);
    inline int getValue(int _row, int _col) const { return m_cells[_row][_col].getValue(); }
    inline void setValue(int _row, int _col, int _val) { m_cells[_row][_col].setValue(_val); }
    inline size_t notesCount(int _row, int _col) const { return m_cells[_row][_col].notesCount(); }
    inline Cell::Notes getNotes(int _row, int _col) const { return m_cells[_row][_col].getNotes(); }
    inline Cell::Notes getNotes(int _i, int _j, int gType)
    {
        int r, c;
        translateCoordinates(_i, _j, r, c, gType);
        return m_cells[r][c].getNotes();
    }
    inline bool hasNote(int _row, int _col, int _note) const { return m_cells[_row][_col].hasNote(_note); }
    inline bool hasNote(int _i, int _j, int _note, int gType) const
    {
        int r, c;
        translateCoordinates(_i, _j, r, c, gType);
        return m_cells[r][c].hasNote(_note);
    }
    inline void setNote(int _row, int _col, int _note, bool _active)
    {
        m_cells[_row][_col].setNote(_note, _active);
        if (m_summary)
            m_summary->updateNote(_row, _col, _note - 1, _active);
    }
    void setNotes(int _i, int _j, int _gType, const Cell::Notes &_notes);
    int clearNotes(int _row, int _col);
    bool isAllowedValue(int _row, int _col, int _val);
    bool isFull();
    void fillNotes();
    void clearNotes();
    int countNotes(int _note, int _i, int _gType);
    bool checkAllNotes();
    int clearNotesCascade(int _row, int _col, int _value, CellLogs *cellLogs = nullptr, bool *check = nullptr);
    int clearRowNotes(
        int _row, int _note, CellLogs *cellLogs = nullptr,
        const std::function<bool(int)> &_clear = [](int) { return true; });
    int clearColNotes(
        int _col, int _note, CellLogs *cellLogs = nullptr,
        const std::function<bool(int)> &_clear = [](int) { return true; });
    int clearBlockNotes(
        int _blk, int _note, CellLogs *cellLogs = nullptr,
        const std::function<bool(int, int, int)> &_clear = [](int, int, int) { return true; });
    std::string getNotesSignature();
    static void forAllCells(const std::function<bool(int, int, int, int)> &_callback);
    static void forIntersections(const std::vector<std::pair<int, int>> &_cells,
                                 const std::function<void(int, int)> &_callback);
    bool compareValues(int *_pValues);
    bool compareValues(const Grid &_grid);
    bool compareNotes(const Grid &_grid);
    void dump(int _dumpFlags = D_VALUES | D_NOTES, const std::string &_empty = ".", const std::string &_numSep = "  ",
              const std::string &_colSep = "    ", const std::string &_lineSep = "");

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
