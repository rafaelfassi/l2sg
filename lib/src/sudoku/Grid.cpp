#include "Grid.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>

// clang-format off

// Creating matrices with known calculations' results to improve performance. 

// Matrix from:
// r = (j / 3) + (i / 3) * 3;
// c = (j % 3) + ((i - (r / 3) * 3) * 3);
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
    for (int r = 0; r < 9; ++r)
    {
        m_notesByRow[r].reserve(9);
        m_notesByCol[r].reserve(9);
        m_notesByBlk[r].reserve(9);
        for (int c = 0; c < 9; ++c)
        {
            const auto [b, e] = g_blockElem2RowCol[r][c];
            for (int nIdx = 0; nIdx < 9; ++nIdx)
            {
                if (pGrid.hasNote(r, c, nIdx + 1))
                {
                    if (!m_colsByRowNote[r][nIdx].any())
                    {
                        m_notesByRow[r].push_back(nIdx);
                    }
                    if (!m_rowsByColNote[c][nIdx].any())
                    {
                        m_notesByCol[c].push_back(nIdx);
                    }
                    if (!m_elmsByBlkNote[b][nIdx].any())
                    {
                        m_notesByBlk[b].push_back(nIdx);
                    }

                    m_colsByRowNote[r][nIdx].set(c, true);
                    m_rowsByColNote[c][nIdx].set(r, true);
                    m_elmsByBlkNote[b][nIdx].set(e, true);
                }
            }
        }
    }
}

void Grid::Summary::updateNote(int r, int c, int nIdx, bool active)
{
    if (active == m_colsByRowNote[r][nIdx].test(c))
        return;

    const auto &be = g_blockElem2RowCol[r][c];

    const auto updateSets = [&]()
    {
        m_colsByRowNote[r][nIdx].set(c, active);
        m_rowsByColNote[c][nIdx].set(r, active);
        m_elmsByBlkNote[be.first][nIdx].set(be.second, active);
    };

    if (active)
    {
        if (!m_colsByRowNote[r][nIdx].any())
            m_notesByRow[r].push_back(nIdx);
        if (!m_rowsByColNote[c][nIdx].any())
            m_notesByCol[c].push_back(nIdx);
        if (!m_elmsByBlkNote[be.first][nIdx].any())
            m_notesByBlk[be.first].push_back(nIdx);

        updateSets();
    }
    else
    {
        const auto removeNoteFromVec = [nIdx](std::vector<int> &vec)
        {
            auto it = std::find(vec.begin(), vec.end(), nIdx);
            if (it != vec.end())
            {
                if (it != vec.end() - 1)
                    *it = vec.back();
                vec.pop_back();
            }
        };

        updateSets();

        if (!m_colsByRowNote[r][nIdx].any())
            removeNoteFromVec(m_notesByRow[r]);
        if (!m_rowsByColNote[c][nIdx].any())
            removeNoteFromVec(m_notesByCol[c]);
        if (!m_elmsByBlkNote[be.first][nIdx].any())
            removeNoteFromVec(m_notesByBlk[be.first]);
    }
}

void Grid::Summary::updateNotes(int r, int c, const Cell::Notes &oldNotes, const Cell::Notes &newNotes)
{
    const Cell::Notes changedNotes(oldNotes ^ newNotes);
    for (const auto nIdx : utils::bitset_it(changedNotes))
    {
        const bool active(newNotes.test(nIdx));
        updateNote(r, c, nIdx, active);
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
    int r = 0;
    int c = 0;
    for (size_t i = 0; i < notes.size(); ++i)
    {
        const auto ch = notes[i];
        if (std::isspace(ch))
            continue;

        if (isValidDigit(ch))
        {
            setNote(r, c, char2Int(ch), true);
        }
        else if (ch == '|')
        {
            if (++c == 9)
            {
                c = 0;
                if (++r == 9)
                    break;
            }
        }
    }
}

void Grid::fillBoard(const std::string &board)
{
    int r(0);
    int c(0);
    size_t i(0);
    while (i < board.size())
    {
        if (c == 9)
        {
            c = 0;
            if (++r == 9)
                break;
        }

        if (!isValidDigit(board[i]))
        {
            ++i;
            continue;
        }

        size_t notesCount(0);
        while (i < board.size())
        {
            const auto noteChar = board[i++];
            if (std::isspace(noteChar))
                break;
            if (isValidDigit(noteChar))
            {
                setNote(r, c, char2Int(noteChar), true);
                if(++notesCount == 9)
                    break;
            }
        }
        ++c;
    }

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (auto &cell = getCell(i, j); cell.notesCount() == 1)
            {
                const auto note = utils::bitset_it(cell.getNotes()).front() + 1;
                int b = getBlockNumber(i, j);
                if ((countNotes(note, i, GT_ROW) == 1) && (countNotes(note, j, GT_COL) == 1) &&
                    (countNotes(note, b, GT_BLK) == 1))
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

void Grid::setNotes(int _i, int _j, int _gType, const Cell::Notes &_notes)
{
    int r, c;
    translateCoordinates(_i, _j, r, c, _gType);
    auto &cell = getCell(r, c);

    if (m_summary)
    {
        m_summary->updateNotes(r, c, cell.getNotes(), _notes);
    }

    cell.setNotes(_notes);
}

int Grid::clearNotes(int _row, int _col)
{
    const auto &notes = getNotes(_row, _col);
    const int count = notes.count();

    if (m_summary)
    {
        m_summary->updateNotes(_row, _col, notes, Cell::Notes());
    }

    m_cells[_row][_col].clearNotes();

    return count;
}

bool Grid::isAllowedValue(int _row, int _col, int _val)
{
    // If the cell already has the value that is being tested, that's all right.
    if (getValue(_row, _col) == _val)
        return true;

    // If the above validation has passed and the cell is not empty, it's trying to change its value.
    if (getValue(_row, _col) != 0)
        return false;

    // If the code hits here, the cell is empty.
    // The value for the cell is allowed if there's no other cell in the same row, column or block
    // with the same value.

    // Check if the value already exists for the column.
    for (int c = 0; c < 9; ++c)
        if (getValue(_row, c) == _val)
            return false;

    // Check if the value already exists for the row.
    for (int r = 0; r < 9; ++r)
        if (getValue(r, _col) == _val)
            return false;

    // Check if the value already exists for the block.
    const auto b = g_blockElem2RowCol[_row][_col].first;
    for (int e = 0; e < 9; ++e)
    {
        const auto [r, c] = g_blockElem2RowCol[b][e];
        if (getValue(r, c) == _val)
            return false;
    }

    // The cell is empty, and no other cell in the same row, column or block has the value.
    return true;
}

bool Grid::isFull()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (getValue(r, c) == 0)
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

int Grid::countNotes(int _note, int _i, int _gType)
{
    int count(0);

    if (_gType == GT_ROW)
    {
        for (int c = 0; c < 9; ++c)
        {
            count += hasNote(_i, c, _note);
        }
    }
    else if (_gType == GT_COL)
    {
        for (int r = 0; r < 9; ++r)
        {
            count += hasNote(r, _i, _note);
        }
    }
    else if (_gType == GT_BLK)
    {
        for (int e = 0; e < 9; ++e)
        {
            const auto [r, c] = g_blockElem2RowCol[_i][e];
            count += hasNote(r, c, _note);
        }
    }

    return count;
}

bool Grid::checkAllNotes()
{
    Cell::Notes rowGroup[9];
    Cell::Notes colGroup[9];
    Cell::Notes blkGroup[9];

    forAllCells(
        [&](int r, int c, int b, int)
        {
            const auto &value(getValue(r, c));
            if (value)
            {
                rowGroup[r].set(value - 1);
                colGroup[c].set(value - 1);
                blkGroup[b].set(value - 1);
            }
            else
            {
                const auto &notes(getNotes(r, c));
                if (!notes.any())
                    return false;
                rowGroup[r] |= notes;
                colGroup[c] |= notes;
                blkGroup[b] |= notes;
            }
            return true;
        });

    for (int i = 0; i < 9; ++i)
    {
        if (!rowGroup[i].all() || !colGroup[i].all() || !blkGroup[i].all())
        {
            return false;
        }
    }

    return true;
}

int Grid::clearNotesCascade(int _row, int _col, int _value, CellLogs *cellLogs, bool *check)
{
    int cleanedCount(clearNotes(_row, _col));

    for (int r = 0; r < 9; ++r)
    {
        if (r == _row)
            continue;
        if (Cell &cell = getCell(r, _col); cell.hasNote(_value))
        {
            setNote(r, _col, _value, false);
            if (check && !cell.hasAnyNote())
            {
                *check = false;
                return 0;
            }
            if (cellLogs)
                cellLogs->emplace_back(r, _col, CellAction::RemovedNote, _value);
            ++cleanedCount;
        }
    }

    for (int c = 0; c < 9; ++c)
    {
        if (c == _col)
            continue;
        if (Cell &cell = getCell(_row, c); cell.hasNote(_value))
        {
            setNote(_row, c, _value, false);
            if (check && !cell.hasAnyNote())
            {
                *check = false;
                return 0;
            }
            if (cellLogs)
                cellLogs->emplace_back(_row, c, CellAction::RemovedNote, _value);
            ++cleanedCount;
        }
    }

    const auto b = g_blockElem2RowCol[_row][_col].first;
    for (int e = 0; e < 9; ++e)
    {
        const auto [r, c] = g_blockElem2RowCol[b][e];
        // The previous loops have cleaned all the candidates from the row and column already.
        if ((r == _row) || (c == _col))
            continue;
        if (Cell &cell = getCell(r, c); cell.hasNote(_value))
        {
            setNote(r, c, _value, false);
            if (check && !cell.hasAnyNote())
            {
                *check = false;
                return 0;
            }
            if (cellLogs)
                cellLogs->emplace_back(r, c, CellAction::RemovedNote, _value);
            ++cleanedCount;
        }
    }

    if (check)
    {
        *check = true;
    }

    return cleanedCount;
}

int Grid::clearRowNotes(int _row, int _note, CellLogs *cellLogs, const std::function<bool(int)> &_clear)
{
    int count(0);
    for (int c = 0; c < 9; ++c)
    {
        if (hasNote(_row, c, _note) && _clear(c))
        {
            setNote(_row, c, _note, false);
            if (cellLogs)
                cellLogs->emplace_back(_row, c, CellAction::RemovedNote, _note);
            ++count;
        }
    }
    return count;
}

int Grid::clearColNotes(int _col, int _note, CellLogs *cellLogs, const std::function<bool(int)> &_clear)
{
    int count(0);
    for (int r = 0; r < 9; ++r)
    {
        if (hasNote(r, _col, _note) && _clear(r))
        {
            setNote(r, _col, _note, false);
            if (cellLogs)
                cellLogs->emplace_back(r, _col, CellAction::RemovedNote, _note);
            ++count;
        }
    }
    return count;
}

int Grid::clearBlockNotes(int _blk, int _note, CellLogs *cellLogs, const std::function<bool(int, int, int)> &_clear)
{
    int count(0);
    for (int e = 0; e < 9; ++e)
    {
        const auto [r, c] = g_blockElem2RowCol[_blk][e];
        if (hasNote(r, c, _note) && _clear(e, r, c))
        {
            setNote(r, c, _note, false);
            if (cellLogs)
                cellLogs->emplace_back(r, c, CellAction::RemovedNote, _note);
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
    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            const auto [b, e] = g_blockElem2RowCol[r][c];
            if (!_callback(r, c, b, e))
            {
                return;
            }
        }
    }
}

void Grid::forIntersections(const std::vector<std::pair<int, int>> &_cells, const std::function<void(int, int)> &_callback)
{
    std::bitset<9> blocks;
    std::bitset<9> rows;
    std::bitset<9> cols;
    std::bitset<3> bands;
    std::bitset<3> stacks;
    std::bitset<9> blockRow[9];
    std::bitset<9> blockCol[9];

    for (const auto &[r, c] : _cells)
    {
        const int b = getBlockNumber(r, c);
        blocks.set(b);
        rows.set(r);
        cols.set(c);
        bands.set(r / 3);
        stacks.set(c / 3);
        blockRow[b].set(r);
        blockCol[b].set(c);
    }

    if (blocks.count() == 1)
    {
        const auto b = getBlockNumber(_cells[0].first, _cells[0].second);
        for (int e = 0; e < 9; ++e)
        {
            int r, c;
            translateCoordinates(b, e, r, c, GT_BLK);
            _callback(r, c);
        }
    }
    else if (bands.count() == 1)
    {
        const auto startRow((_cells[0].first / 3) * 3);
        const auto endRow(startRow + 3);
        for (int r = startRow; r < endRow; ++r)
        {
            if (rows.test(r))
            {
                for (int c = 0; c < 9; ++c)
                {
                    const int b = getBlockNumber(r, c);
                    if (blocks.test(b) && !blockRow[b].test(r))
                        _callback(r, c);
                }
            }
        }
    }
    else if (stacks.count() == 1)
    {
        const auto startCol((_cells[0].second / 3) * 3);
        const auto endCol(startCol + 3);
        for (int c = startCol; c < endCol; ++c)
        {
            if (cols.test(c))
            {
                for (int r = 0; r < 9; ++r)
                {
                    const int b = getBlockNumber(r, c);
                    if (blocks.test(b) && !blockCol[b].test(c))
                        _callback(r, c);
                }
            }
        }
    }
    else
    {
        for (const auto r : utils::bitset_it(rows))
        {
            for (const auto c : utils::bitset_it(cols))
            {
                _callback(r, c);
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

void Grid::dump(int _dumpFlags, const std::string &_empty, const std::string &_numSep, const std::string &_colSep,
                const std::string &_lineSep)
{
    const bool oneLine(_dumpFlags & D_ONE_LINE);

    const auto dumpGridFunc = [&](const std::string &title, std::function<void(int, int, std::ostream &out)> printCell)
    {
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
        dumpGridFunc("Values:",
                     [this, &_empty](int i, int j, std::ostream &out)
                     {
                         if (const int val = getValue(i, j); val)
                             out << val;
                         else
                             out << _empty;
                     });
    }

    if (_dumpFlags & D_NOTES)
    {
        dumpGridFunc("Notes:",
                     [this, &_empty](int i, int j, std::ostream &out)
                     {
                         for (int n = 1; n <= 9; ++n)
                         {
                             if (hasNote(i, j, n))
                                 out << n;
                             else
                                 out << _empty;
                         }
                     });
    }

    if (_dumpFlags & D_BOARD)
    {
        dumpGridFunc("Board:",
                     [this, oneLine](int i, int j, std::ostream &out)
                     {
                         if (const int val = getValue(i, j); val)
                         {
                             out << std::left << std::setfill(' ') << std::setw(oneLine ? 1 : 10) << val;
                         }
                         else
                         {
                             std::stringstream ssNotes;
                             for (int n = 1; n <= 9; ++n)
                             {
                                 if (hasNote(i, j, n))
                                 {
                                     ssNotes << n;
                                 }
                             }
                             out << std::left << std::setfill(' ') << std::setw(oneLine ? 1 : 10) << ssNotes.str();
                         }
                     });
    }
}

} // namespace sudoku