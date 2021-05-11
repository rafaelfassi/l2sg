#include "Grid.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>

// clang-format off

// Creating matrices with known calculations' results to improve performance. 

// Matrix from:
// l = (j / 3) + (i / 3) * 3;
// c = (j % 3) + ((i - (l / 3) * 3) * 3);
// The symmetry of the puzzle makes this map bidirectional. (row->block, block->row)
const std::pair<int, int> sudoku::Grid::g_blockElem2RowCol[9][9] = {
    {{0, 0} ,{0, 1}, {0, 2},   {1, 0}, {1, 1}, {1, 2},   {2, 0}, {2, 1}, {2, 2}},
    {{0, 3} ,{0, 4}, {0, 5},   {1, 3}, {1, 4}, {1, 5},   {2, 3}, {2, 4}, {2, 5}},
    {{0, 6} ,{0, 7}, {0, 8},   {1, 6}, {1, 7}, {1, 8},   {2, 6}, {2, 7}, {2, 8}},

    {{3, 0} ,{3, 1}, {3, 2},   {4, 0}, {4, 1}, {4, 2},   {5, 0}, {5, 1}, {5, 2}},
    {{3, 3} ,{3, 4}, {3, 5},   {4, 3}, {4, 4}, {4, 5},   {5, 3}, {5, 4}, {5, 5}},
    {{3, 6} ,{3, 7}, {3, 8},   {4, 6}, {4, 7}, {4, 8},   {5, 6}, {5, 7}, {5, 8}},

    {{6, 0} ,{6, 1}, {6, 2},   {7, 0}, {7, 1}, {7, 2},   {8, 0}, {8, 1}, {8, 2}},
    {{6, 3} ,{6, 4}, {6, 5},   {7, 3}, {7, 4}, {7, 5},   {8, 3}, {8, 4}, {8, 5}},
    {{6, 6} ,{6, 7}, {6, 8},   {7, 6}, {7, 7}, {7, 8},   {8, 6}, {8, 7}, {8, 8}}};

// clang-format on

namespace sudoku
{

bool isValidDigit(char ch)
{
    return (std::isdigit(ch) && (ch > '0'));
}

int char2Int(char ch)
{
    return ch - '0';
}

Grid::Summary::Summary(const Grid &pGrid)
{
    for (int l = 0; l < 9; ++l)
    {
        m_notesByRow[l].reserve(9);
        m_notesByCol[l].reserve(9);
        m_notesByBlk[l].reserve(9);
        for (int c = 0; c < 9; ++c)
        {
            const auto [b, e] = g_blockElem2RowCol[l][c];
            for (int vIdx = 0; vIdx < 9; ++vIdx)
            {
                if (pGrid.hasNote(l, c, vIdx + 1))
                {
                    if (!m_colsByRowNote[l][vIdx].any())
                    {
                        m_notesByRow[l].push_back(vIdx);
                    }
                    if (!m_rowsByColNote[c][vIdx].any())
                    {
                        m_notesByCol[c].push_back(vIdx);
                    }
                    if (!m_elmsByBlkNote[b][vIdx].any())
                    {
                        m_notesByBlk[b].push_back(vIdx);
                    }

                    m_colsByRowNote[l][vIdx].set(c, true);
                    m_rowsByColNote[c][vIdx].set(l, true);
                    m_elmsByBlkNote[b][vIdx].set(e, true);
                }
            }
        }
    }
}

void Grid::Summary::updateNote(int l, int c, int vIdx, bool active)
{
    if (active == m_colsByRowNote[l][vIdx].test(c))
        return;

    const auto &be = g_blockElem2RowCol[l][c];

    const auto updateSets = [&]() {
        m_colsByRowNote[l][vIdx].set(c, active);
        m_rowsByColNote[c][vIdx].set(l, active);
        m_elmsByBlkNote[be.first][vIdx].set(be.second, active);
    };

    if (active)
    {
        if (!m_colsByRowNote[l][vIdx].any())
            m_notesByRow[l].push_back(vIdx);
        if (!m_rowsByColNote[c][vIdx].any())
            m_notesByCol[c].push_back(vIdx);
        if (!m_elmsByBlkNote[be.first][vIdx].any())
            m_notesByBlk[be.first].push_back(vIdx);

        updateSets();
    }
    else
    {
        const auto removeNoteFromVec = [vIdx](std::vector<int> &vec) {
            auto it = std::find(vec.begin(), vec.end(), vIdx);
            if (it != vec.end())
            {
                if(it != vec.end()-1)
                    *it = vec.back();
                vec.pop_back();
            }
        };

        updateSets();

        if (!m_colsByRowNote[l][vIdx].any())
            removeNoteFromVec(m_notesByRow[l]);
        if (!m_rowsByColNote[c][vIdx].any())
            removeNoteFromVec(m_notesByCol[c]);
        if (!m_elmsByBlkNote[be.first][vIdx].any())
            removeNoteFromVec(m_notesByBlk[be.first]);
    }
}

void Grid::Summary::updateNotes(int l, int c, const Cell::Notes &oldNotes, const Cell::Notes &newNotes)
{
    const Cell::Notes changedNotes(oldNotes ^ newNotes);
    for (const auto vIdx : utils::bitset_it(changedNotes))
    {
        const bool active(newNotes.test(vIdx));
        updateNote(l, c, vIdx, active);
    }
}

void Grid::fillValues(int *_pValues)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            setValue(i, j, _pValues[j + (i * 9)]);
}

bool Grid::fillValues(const std::string &values)
{
    int arr[9 * 9];
    if (fillValuesArrayFormString(values, arr))
    {
        fillValues(arr);
        return true;
    }
    return false;
}

void Grid::fillNotes(const std::string &notes)
{
    int l = 0;
    int c = 0;
    for (size_t i = 0; i < notes.size(); ++i)
    {
        const auto ch = notes[i];
        if (std::isspace(ch))
            continue;

        if (isValidDigit(ch))
        {
            setNote(l, c, char2Int(ch), true);
        }
        else if (ch == '|')
        {
            if (++c == 9)
            {
                c = 0;
                if (++l == 9)
                    break;
            }
        }
    }
}

void Grid::fillBoard(const std::string &board)
{
    int l(0);
    int c(0);
    size_t i(0);
    while (i < board.size())
    {
        if (c == 9)
        {
            c = 0;
            if (++l == 9)
                break;
        }

        if (!isValidDigit(board[i]))
        {
            ++i;
            continue;
        }

        int notesCount(0);
        while (i < board.size())
        {
            const auto noteChar = board[i];
            if (!isValidDigit(noteChar) || (++notesCount > 9))
                break;
            setNote(l, c, char2Int(noteChar), true);
            ++i;
        }

        if (++c == 9)
        {
            c = 0;
            if (++l == 9)
                break;
        }
    }

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (auto &cell = getCell(i, j); cell.notesCount() == 1)
            {
                const auto note = utils::bitset_it(cell.getNotes()).front() + 1;
                int b = getBlockNumber(i, j);
                if ((countNotes(note, i, T_LINE) == 1) && (countNotes(note, j, T_COLUMN) == 1) &&
                    (countNotes(note, b, T_BLOCK) == 1))
                {
                    cell.setValue(note);
                    clearNotesCascade(i, j, note);
                }
            }
        }
    }
}

bool Grid::fillValuesArrayFormString(const std::string &values, int *array)
{
    int x(0);
    for (size_t i = 0; i < values.size(); ++i)
    {
        const auto ch = values[i];

        if (std::isspace(ch))
            continue;

        if (x == 9 * 9)
            break;

        if (isValidDigit(ch))
            array[x++] = char2Int(ch);
        else
            array[x++] = 0;
    }

    return (x == 9 * 9);
}

void Grid::setNotes(int _i, int _j, int _type, const Cell::Notes &_nNotes)
{
    int l, c;
    translateCoordinates(_i, _j, l, c, _type);
    auto &cell = getCell(l, c);

    if (m_summary)
    {
        m_summary->updateNotes(l, c, cell.getNotes(), _nNotes);
    }

    cell.setNotes(_nNotes);
}

int Grid::clearNotes(int _nLin, int _nCol)
{
    const auto &notes = getNotes(_nLin, _nCol);
    const int count = notes.count();

    if (m_summary)
    {
        m_summary->updateNotes(_nLin, _nCol, notes, Cell::Notes());
    }

    m_cells[_nLin][_nCol].clearNotes();

    return count;
}

bool Grid::isAllowedValue(int _nLin, int _nCol, int _nVal)
{
    // If the cell already has the value that is being tested, that's all right.
    if (getValue(_nLin, _nCol) == _nVal)
        return true;

    // If the above validation has passed and the cell is not empty, it's trying to change its value.
    if (getValue(_nLin, _nCol) != 0)
        return false;

    // If the code hits here, the cell is empty.
    // The value for the cell is allowed if there's no other cell in the same row, column or block
    // with the same value.

    // Check if the value already exists for the column.
    for (int c = 0; c < 9; ++c)
        if (getValue(_nLin, c) == _nVal)
            return false;

    // Check if the value already exists for the row.
    for (int l = 0; l < 9; ++l)
        if (getValue(l, _nCol) == _nVal)
            return false;

    // Check if the value already exists for the block.
    const auto b = g_blockElem2RowCol[_nLin][_nCol].first;
    for (int e = 0; e < 9; ++e)
    {
        const auto [l, c] = g_blockElem2RowCol[b][e];
        if (getValue(l, c) == _nVal)
            return false;
    }

    // The cell is empty, and no other cell in the same row, column or block has the value.
    return true;
}

bool Grid::isFull()
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (getValue(i, j) == 0)
                return false;
    return true;
}

void Grid::fillNotes()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (getValue(i, j) == 0)
            {
                for (int x = 1; x <= 9; ++x)
                {
                    if (isAllowedValue(i, j, x))
                        setNote(i, j, x, true);
                }
            }
        }
    }
}

void Grid::clearNotes()
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            clearNotes(i, j);
}

int Grid::countNotes(int _nVal, int _i, int _type)
{
    int count(0);

    if (_type == T_LINE)
    {
        for (int c = 0; c < 9; ++c)
        {
            count += hasNote(_i, c, _nVal);
        }
    }
    else if (_type == T_COLUMN)
    {
        for (int l = 0; l < 9; ++l)
        {
            count += hasNote(l, _i, _nVal);
        }
    }
    else if (_type == T_BLOCK)
    {
        for (int e = 0; e < 9; ++e)
        {
            const auto [l, c] = g_blockElem2RowCol[_i][e];
            count += hasNote(l, c, _nVal);
        }
    }

    return count;
}

int Grid::clearNotesCascade(int _nLin, int _nCol, int _nValue,  CellLogs *cellLogs, bool *check)
{
    int cleanedCount(clearNotes(_nLin, _nCol));

    for (int l = 0; l < 9; ++l)
    {
        if (l == _nLin)
            continue;
        if (Cell &cell = getCell(l, _nCol); cell.hasNote(_nValue))
        {
            setNote(l, _nCol, _nValue, false);
            if (check && !cell.hasAnyNote())
            {
                *check = false;
                return 0;
            }
            if (cellLogs)
                cellLogs->emplace_back(l, _nCol, CellAction::RemovedNote, _nValue);
            ++cleanedCount;
        }
    }

    for (int c = 0; c < 9; ++c)
    {
        if (c == _nCol)
            continue;
        if (Cell &cell = getCell(_nLin, c); cell.hasNote(_nValue))
        {
            setNote(_nLin, c, _nValue, false);
            if (check && !cell.hasAnyNote())
            {
                *check = false;
                return 0;
            }
            if (cellLogs)
                cellLogs->emplace_back(_nLin, c, CellAction::RemovedNote, _nValue);
            ++cleanedCount;
        }
    }

    const auto b = g_blockElem2RowCol[_nLin][_nCol].first;
    for (int e = 0; e < 9; ++e)
    {
        const auto [l, c] = g_blockElem2RowCol[b][e];
        // The previous loops have cleaned all the candidates from the row and column already.
        if ((l == _nLin) || (c == _nCol))
            continue;
        if (Cell &cell = getCell(l, c); cell.hasNote(_nValue))
        {
            setNote(l, c, _nValue, false);
            if (check && !cell.hasAnyNote())
            {
                *check = false;
                return 0;
            }
            if (cellLogs)
                cellLogs->emplace_back(l, c, CellAction::RemovedNote, _nValue);
            ++cleanedCount;
        }
    }

    if (check)
    {
        *check = true;
    }

    return cleanedCount;
}

int Grid::clearRowNotes(int _row, int _val, CellLogs *cellLogs, const std::function<bool(int)> &_clear)
{
    int count(0);
    for (int c = 0; c < 9; ++c)
    {
        if (hasNote(_row, c, _val) && _clear(c))
        {
            setNote(_row, c, _val, false);
            if (cellLogs)
                cellLogs->emplace_back(_row, c, CellAction::RemovedNote, _val);
            ++count;
        }
    }
    return count;
}

int Grid::clearColNotes(int _col, int _val, CellLogs *cellLogs, const std::function<bool(int)> &_clear)
{
    int count(0);
    for (int l = 0; l < 9; ++l)
    {
        if (hasNote(l, _col, _val) && _clear(l))
        {
            setNote(l, _col, _val, false);
            if (cellLogs)
                cellLogs->emplace_back(l, _col, CellAction::RemovedNote, _val);
            ++count;
        }
    }
    return count;
}

int Grid::clearBlockNotes(int _blk, int _val, CellLogs *cellLogs, const std::function<bool(int, int, int)> &_clear)
{
    int count(0);
    for (int e = 0; e < 9; ++e)
    {
        const auto [l, c] = g_blockElem2RowCol[_blk][e];
        if (hasNote(l, c, _val) && _clear(e, l, c))
        {
            setNote(l, c, _val, false);
            if (cellLogs)
                cellLogs->emplace_back(l, c, CellAction::RemovedNote, _val);
            ++count;
        }
    }
    return count;
}

std::string Grid::getNotesSignature()
{
    std::string result;

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            result += getCell(i, j).getNotes().to_string();
        }
    }

    return result;
}

void Grid::forAllCells(const std::function<bool(int, int, int, int)> &_callback)
{
    for (int l = 0; l < 9; ++l)
    {
        for (int c = 0; c < 9; ++c)
        {
            const auto [b, e] = g_blockElem2RowCol[l][c];
            if (!_callback(l, c, b, e))
            {
                return;
            }
        }
    }
}

bool Grid::compareValues(int *_pValues)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (getValue(i, j) != _pValues[j + (i * 9)])
                return false;
    return true;
}

bool Grid::compareValues(const Grid &_grid)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (getValue(i, j) != _grid.getValue(i, j))
                return false;
    return true;
}

bool Grid::compareNotes(const Grid &_grid)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (getNotes(i, j) != _grid.getNotes(i, j))
                return false;
    return true;
}

void Grid::dump(int _dumpFlags, const std::string &_empty, const std::string &_numSep,
                const std::string &_colSep, const std::string &_lineSep)
{
    const bool oneLine(_dumpFlags & D_ONE_LINE);

    const auto dumpGridFunc = [&](const std::string &title,
                                  std::function<void(int, int, std::ostream &out)> printCell) {
        if (!oneLine)
            std::cout << title << std::endl;

        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                printCell(i, j, std::cout);

                if (j < 8)
                {
                    if (j % 3 == 2)
                        std::cout << _colSep;
                    else
                        std::cout << _numSep;
                }
            }

            if (i < 8)
            {
                std::cout << _lineSep;
                if (!oneLine)
                {
                    std::cout << std::endl;
                    if (i % 3 == 2)
                        std::cout << std::endl;
                }
            }
        }
        std::cout << std::endl;
        if (!oneLine)
        {
            std::cout << std::endl;
        }
    };

    if (_dumpFlags & D_VALUES)
    {
        dumpGridFunc("Values:", [this, &_empty](int i, int j, std::ostream &out) {
            if (const int val = getValue(i, j); val)
                out << val;
            else
                out << _empty;
        });
    }

    if (_dumpFlags & D_NOTES)
    {
        dumpGridFunc("Notes:", [this, &_empty](int i, int j, std::ostream &out) {
            for (int x = 1; x <= 9; ++x)
            {
                if (hasNote(i, j, x))
                    out << x;
                else
                    out << _empty;
            }
        });
    }

    if (_dumpFlags & D_BOARD)
    {
        dumpGridFunc("Board:", [this, oneLine](int i, int j, std::ostream &out) {
            if (const int val = getValue(i, j); val)
            {
                out << std::left << std::setfill(' ') << std::setw(oneLine ? 1 : 10) << val;
            }
            else
            {
                std::stringstream ssNotes;
                for (int x = 1; x <= 9; ++x)
                {
                    if (hasNote(i, j, x))
                    {
                        ssNotes << x;
                    }
                }
                out << std::left << std::setfill(' ') << std::setw(oneLine ? 1 : 10) << ssNotes.str();
            }
        });
    }
}

} // namespace sudoku