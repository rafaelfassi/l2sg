#include "Grid.h"

#include <iostream>

// clang-format off

// Creating matrices with known calculations' results to improve performance. 

// Matrix from:
// b = (c / 3) + (l / 3) * 3
int g_rowCol2Block[9][9] = {
    {0 ,0, 0,   1, 1, 1,   2, 2, 2},
    {0 ,0, 0,   1, 1, 1,   2, 2, 2},
    {0 ,0, 0,   1, 1, 1,   2, 2, 2},

    {3 ,3, 3,   4, 4, 4,   5, 5, 5},
    {3 ,3, 3,   4, 4, 4,   5, 5, 5},
    {3 ,3, 3,   4, 4, 4,   5, 5, 5},

    {6 ,6, 6,   7, 7, 7,   8, 8, 8},
    {6 ,6, 6,   7, 7, 7,   8, 8, 8},
    {6 ,6, 6,   7, 7, 7,   8, 8, 8}};

// Matrix from:
// l = (j / 3) + (i / 3) * 3;
// c = (j % 3) + ((i - (l / 3) * 3) * 3);
std::pair<int, int> g_blockElem2RowCol[9][9] = {
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

Cell &Grid::getTranslatedCell(int _i, int _j, int type)
{
    int l, c;
    translateCoordinates(_i, _j, l, c, type);
    return getCell(l, c);
}

void Grid::translateCoordinates(int _i, int _j, int &_l, int &_c, int type)
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
            const auto &rowCol = g_blockElem2RowCol[_i][_j];
            _l = rowCol.first;
            _c = rowCol.second;
            break;
    }
}

std::pair<int, int> Grid::getCellCoordinates(int _cellNum)
{
    const int l = _cellNum / 9;
    const int c = _cellNum % 9;
    return std::make_pair(l, c);
}

std::pair<int, int> Grid::getBlockStartCoordinates(int _b)
{
    const int l = (_b / 3) * 3;
    const int c = (_b % 3) * 3;
    return std::make_pair(l, c);
}

int Grid::getBlockNumber(int _l, int _c) const
{
    return g_rowCol2Block[_l][_c];
}

void Grid::fillValues(int *_pValues)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            setValue(i, j, _pValues[j + (i * 9)]);
}

void Grid::fillValues(const std::string &values)
{
    int arr[9 * 9];
    if (fillValuesArrayFormString(values, arr))
        fillValues(arr);
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

        if (std::isdigit(ch) && (ch > '0'))
        {
            setNote(l, c, (ch - '0'), true);
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

bool Grid::fillValuesArrayFormString(const std::string &values, int *array)
{
    int x(0);
    for (size_t i = 0; i < values.size(); ++i)
    {
        const auto ch = values[i];

        if (std::isspace(ch))
            continue;

        if (x == 9 * 9)
            return false;

        if (std::isdigit(ch) && ch > '0')
            array[x++] = (ch - '0'); // '0' == 48
        else
            array[x++] = 0;
    }

    return (x == 9 * 9);
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
    const auto b = g_rowCol2Block[_nLin][_nCol];
    for (int e = 0; e < 9; ++e)
    {
        const auto &rowCol = g_blockElem2RowCol[b][e];
        if (getValue(rowCol.first, rowCol.second) == _nVal)
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
            getCell(i, j).clearNotes();
}

bool Grid::clearNotesCascade(int _nLin, int _nCol, int _nValue)
{
    clearNotes(_nLin, _nCol);

    for (int l = 0; l < 9; ++l)
    {
        if (l == _nLin)
            continue;
        if (Cell &cell = getCell(l, _nCol); cell.hasNote(_nValue))
        {
            cell.setNote(_nValue, false);
            if (!cell.hasAnyNote())
            {
                return false;
            }
        }
    }

    for (int c = 0; c < 9; ++c)
    {
        if (c == _nCol)
            continue;
        if (Cell &cell = getCell(_nLin, c); cell.hasNote(_nValue))
        {
            cell.setNote(_nValue, false);
            if (!cell.hasAnyNote())
            {
                return false;
            }
        }
    }

    const auto b = g_rowCol2Block[_nLin][_nCol];
    for (int e = 0; e < 9; ++e)
    {
        const auto &rowCol = g_blockElem2RowCol[b][e];
        // The previous loops have cleaned all the candidates from the row and column already.
        if ((rowCol.first == _nLin) || (rowCol.second == _nCol))
            continue;
        if (Cell &cell = getCell(rowCol.first, rowCol.second); cell.hasNote(_nValue))
        {
            cell.setNote(_nValue, false);
            if (!cell.hasAnyNote())
            {
                return false;
            }
        }
    }

    return true;
}

void Grid::clearRowNotes(int _row, int _val, const std::function<bool(int)> &_clear)
{
    for (int c = 0; c < 9; ++c)
    {
        if (hasNote(_row, c, _val) && _clear(c))
            setNote(_row, c, _val, false);
    }
}

void Grid::clearColNotes(int _col, int _val, const std::function<bool(int)> &_clear)
{
    for (int l = 0; l < 9; ++l)
    {
        if (hasNote(l, _col, _val) && _clear(l))
            setNote(l, _col, _val, false);
    }
}

void Grid::clearBlockNotes(int _blk, int _val, const std::function<bool(int, int)> &_clear)
{
    for (int e = 0; e < 9; ++e)
    {
        const auto &rowCol = g_blockElem2RowCol[_blk][e];
        if (hasNote(rowCol.first, rowCol.second, _val) && _clear(rowCol.first, rowCol.second))
            setNote(rowCol.first, rowCol.second, _val, false);
    }
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

void Grid::forAllCells(const std::function<bool(int, int, int)> &_callback)
{
    for (int l = 0; l < 9; ++l)
    {
        for (int c = 0; c < 9; ++c)
        {
            if (!_callback(l, c, g_rowCol2Block[l][c]))
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

void Grid::dump(int _dumpFlags, const std::string &_null, const std::string &_numSep,
                const std::string &_lineSep, const std::string &_colSep)
{
    if (_dumpFlags & D_VALUES)
    {
        std::cout << "Values:" << std::endl;
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                bool lastOfCol((j % 3) == 2);
                int val = getValue(i, j);
                if (val)
                    std::cout << val << (lastOfCol ? _colSep : _numSep);
                else
                    std::cout << _null << (lastOfCol ? _colSep : _numSep);
            }

            if (i < 8)
            {
                std::cout << _lineSep;
                if (!(_dumpFlags & D_ONE_LINE))
                {
                    std::cout << std::endl;
                    if (i % 3 == 2)
                        std::cout << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }

    if (_dumpFlags & D_NOTES)
    {
        std::cout << "Notes:" << std::endl;
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                for (int x = 1; x <= 9; ++x)
                {
                    if (hasNote(i, j, x))
                        std::cout << x;
                    else
                        std::cout << _null;
                }

                if (j % 3 == 2)
                    std::cout << _colSep;
                else
                    std::cout << _numSep;
            }

            if (i < 8)
            {
                std::cout << _lineSep;
                if (!(_dumpFlags & D_ONE_LINE))
                {
                    std::cout << std::endl;
                    if (i % 3 == 2)
                        std::cout << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }
}

} // namespace sudoku