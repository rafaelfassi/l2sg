#include "Grid.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>

// clang-format off

// Creating matrices with known calculations' results to improve performance. 

// Matrix from:
// r = (j / 3) + (i / 3) * 3;
// c = (j % 3) + ((i - (r / 3) * 3) * 3);
// The symmetry of the puzzle makes this map bidirectional. (row->block, block->row)
const std::pair<int, int> l2sg::Grid::g_blockElem2RowCol[9][9] = {
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

namespace l2sg
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
        for (int c = 0; c < 9; ++c)
        {
            const auto [b, e] = g_blockElem2RowCol[r][c];
            for (int nIdx = 0; nIdx < 9; ++nIdx)
            {
                if (pGrid.hasNote(r, c, nIdx + 1))
                {
                    m_notesByRow[r].set(nIdx, true);
                    m_notesByCol[c].set(nIdx, true);
                    m_notesByBlk[b].set(nIdx, true);
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
            m_notesByRow[r].set(nIdx, true);
        if (!m_rowsByColNote[c][nIdx].any())
            m_notesByCol[c].set(nIdx, true);
        if (!m_elmsByBlkNote[be.first][nIdx].any())
            m_notesByBlk[be.first].set(nIdx, true);

        updateSets();
    }
    else
    {
        updateSets();

        if (!m_colsByRowNote[r][nIdx].any())
            m_notesByRow[r].set(nIdx, false);
        if (!m_rowsByColNote[c][nIdx].any())
            m_notesByCol[c].set(nIdx, false);
        if (!m_elmsByBlkNote[be.first][nIdx].any())
            m_notesByBlk[be.first].set(nIdx, false);
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

bool Grid::fillBoard(const std::string &board)
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
                if (++notesCount == 9)
                    break;
            }
        }
        ++c;
    }

    if (r != 9)
        return false;

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

    return true;
}

bool Grid::fillAny(const std::string &inStr)
{
    if (inStr.size() < 81)
        return false;

    int cntNum(0);
    std::unordered_set<char> foundChars;

    for (const auto ch : inStr)
    {
        if (std::isspace(ch))
            continue;

        if (isValidDigit(ch))
            ++cntNum;
        else
            foundChars.insert(ch);
    }

    // Less or equal 81 values, and only one character for empty positions (usually '.' or '0')?
    if ((cntNum <= 81) && (foundChars.size() <= 1))
    {
        return fillValues(inStr);
    }

    if ((cntNum >= 81) && (cntNum <= 9 * 81))
    {
        return fillBoard(inStr);
    }

    return false;
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

bool Grid::isEmpty()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (getValue(r, c))
                return false;
    return true;
}

bool Grid::isNotesEmpty()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if ((getValue(r, c) == 0) && getNotes(r, c).any())
                return false;
    return true;
}

bool Grid::hasFullNotes()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if ((getValue(r, c) == 0) && getNotes(r, c).none())
                return false;
    return true;
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

bool Grid::checkAllValues(CellLogs *cellLogs)
{
    Grid::Group rows[9];
    Grid::Group cols[9];
    Grid::Group blks[9];
    bool ok(true);

    forAllCells(
        [&](int r, int c, int b, int)
        {
            const auto vIdx(getValue(r, c) - 1);
            if (vIdx != -1)
            {
                ok &= !rows[r].test(vIdx);
                if (ok)
                    rows[r].set(vIdx);

                ok &= !cols[c].test(vIdx);
                if (ok)
                    cols[c].set(vIdx);

                ok &= !blks[b].test(vIdx);
                if (ok)
                    blks[b].set(vIdx);

                if (!ok && cellLogs)
                {
                    cellLogs->emplace_back(r, c, CellAction::ConflictedValue, vIdx + 1);
                }
            }
            return ok;
        });

    return ok;
}

bool Grid::checkAllNotes(CellLogs *cellLogs)
{
    Cell::Notes rowGroup[9];
    Cell::Notes colGroup[9];
    Cell::Notes blkGroup[9];

    forAllCells(
        [&](int r, int c, int b, int)
        {
            const auto vIdx(getValue(r, c) - 1);
            if (vIdx != -1)
            {
                rowGroup[r].set(vIdx);
                colGroup[c].set(vIdx);
                blkGroup[b].set(vIdx);
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
            if (cellLogs)
            {
                if (!rowGroup[i].all())
                {
                    rowGroup[i].flip();
                    const auto v = utils::bitset_it(rowGroup[i]).front() + 1;
                    cellLogs->emplace_back(i, 0, CellAction::MissingNoteRow, v);
                }
                else if (!colGroup[i].all())
                {
                    colGroup[i].flip();
                    const auto v = utils::bitset_it(colGroup[i]).front() + 1;
                    cellLogs->emplace_back(0, i, CellAction::MissingNoteCol, v);
                }
                else if (!blkGroup[i].all())
                {
                    blkGroup[i].flip();
                    const auto v = utils::bitset_it(blkGroup[i]).front() + 1;
                    const auto &[r, c] = g_blockElem2RowCol[i][0];
                    cellLogs->emplace_back(r, c, CellAction::MissingNoteBlk, v);
                }
            }
            return false;
        }
    }

    return true;
}

bool Grid::checkAll(CellLogs *cellLogs)
{
    return (checkAllValues(cellLogs) && checkAllNotes(cellLogs));
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

bool Grid::removeNotesFromIntersections(int n, const std::vector<std::pair<int, int>> &_cells, CellLogs *cellLogs)
{
    bool changed(false);
    std::bitset<9> blocks;
    std::bitset<9> rows;
    std::bitset<9> cols;
    std::bitset<3> bands;
    std::bitset<3> stacks;
    std::bitset<9> blockRow[9];
    std::bitset<9> blockCol[9];

    const auto isNotTheGivens = [&_cells](const int r, const int c) -> bool
    { return (std::find(_cells.begin(), _cells.end(), std::make_pair(r, c)) == _cells.end()); };

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
            if (hasNote(r, c, n) && isNotTheGivens(r, c))
            {
                setNote(r, c, n, false);
                changed = true;
                if (cellLogs)
                    cellLogs->emplace_back(r, c, CellAction::RemovedNote, n);
            }
        }
    }
    else if (bands.count() == 1)
    {
        for (const int r : utils::bitset_it(rows))
        {
            for (int c = 0; c < 9; ++c)
            {
                const int b = getBlockNumber(r, c);
                if (blocks.test(b) && !blockRow[b].test(r) && hasNote(r, c, n) && isNotTheGivens(r, c))
                {
                    setNote(r, c, n, false);
                    changed = true;
                    if (cellLogs)
                        cellLogs->emplace_back(r, c, CellAction::RemovedNote, n);
                }
            }
        }
    }
    else if (stacks.count() == 1)
    {
        for (const int c : utils::bitset_it(cols))
        {
            for (int r = 0; r < 9; ++r)
            {
                const int b = getBlockNumber(r, c);
                if (blocks.test(b) && !blockCol[b].test(c) && hasNote(r, c, n) && isNotTheGivens(r, c))
                {
                    setNote(r, c, n, false);
                    changed = true;
                    if (cellLogs)
                        cellLogs->emplace_back(r, c, CellAction::RemovedNote, n);
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
                if (hasNote(r, c, n) && isNotTheGivens(r, c))
                {
                    setNote(r, c, n, false);
                    changed = true;
                    if (cellLogs)
                        cellLogs->emplace_back(r, c, CellAction::RemovedNote, n);
                }
            }
        }
    }

    return changed;
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

void Grid::dump(std::ostream &_out, int _dumpFlags, const std::string &_empty, const std::string &_numSep,
                const std::string &_colSep, const std::string &_lineSep)
{
    const bool oneLine(_dumpFlags & D_ONE_LINE);
    const auto dumpGridFunc = [&](std::function<void(int, int, std::ostream &out)> printCell)
    {
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                printCell(i, j, _out);

                if (j < 8)
                {
                    if (j % 3 == 2)
                        _out << _colSep;
                    else
                        _out << _numSep;
                }
            }

            if (i < 8)
            {
                _out << _lineSep;
                if (!oneLine)
                {
                    _out << std::endl;
                    if (i % 3 == 2)
                        _out << std::endl;
                }
            }
        }

        if (!oneLine)
        {
            _out << std::endl;
        }
    };

    if (_dumpFlags & D_VALUES)
    {
        dumpGridFunc(
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
        dumpGridFunc(
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
        dumpGridFunc(
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

} // namespace l2sg