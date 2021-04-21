#include "grid.h"

#include <iostream>
#include <unordered_set>

// clang-format off

// Table from:
// b = (c / 3) + (l / 3) * 3
int g_rowCol2BlockNumber[9][9] = {
    {0 ,0, 0,   1, 1, 1,   2, 2, 2},
    {0 ,0, 0,   1, 1, 1,   2, 2, 2},
    {0 ,0, 0,   1, 1, 1,   2, 2, 2},

    {3 ,3, 3,   4, 4, 4,   5, 5, 5},
    {3 ,3, 3,   4, 4, 4,   5, 5, 5},
    {3 ,3, 3,   4, 4, 4,   5, 5, 5},

    {6 ,6, 6,   7, 7, 7,   8, 8, 8},
    {6 ,6, 6,   7, 7, 7,   8, 8, 8},
    {6 ,6, 6,   7, 7, 7,   8, 8, 8}};

// Table from:
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

Grid::Grid()
{
}

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
            // _l = (_j / 3) + (_i / 3) * 3;
            // _c = (_j % 3) + ((_i - (_l / 3) * 3) * 3);
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
    return g_rowCol2BlockNumber[_l][_c];
    // return (_c / 3) + (_l / 3) * 3;
}

void Grid::setValues(int *_pValues)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            setValue(i, j, _pValues[j + (i * 9)]);
}

void Grid::setValues(const std::string &values)
{
    int arr[9 * 9];
    if (fillArrayFormString(values, arr))
        setValues(arr);
}

bool Grid::fillArrayFormString(const std::string &values, int *array)
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
            array[x++] = (ch - '0'); // Converts the character to int. ('0' == 48)
        else
            array[x++] = 0;
    }

    return (x == 9 * 9);
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

void Grid::dump(int _dumpFlags, const std::string &_null, const std::string &_numSep,
                const std::string &_lineSep, const std::string &_colSep)
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

        if (i < 9 - 1)
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

    if (_dumpFlags & D_ANOTATION)
    {
        std::cout << "Notes:" << std::endl;
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                for (int x = 1; x <= 9; ++x)
                {
                    if (getNoteVisible(i, j, x))
                        std::cout << x;
                    else
                        std::cout << ".";
                }
                std::cout << "  ";
                if (j % 3 == 2)
                    std::cout << "  ";
            }
            std::cout << std::endl;
            if (i % 3 == 2)
                std::cout << std::endl;
        }
    }
}

bool Grid::isAllowedValue(int _nLin, int _nCol, int _nVal)
{
    int l, c, lr, cr;

    /*
      Se a posicao possui o numero que esta sendo testado.
      O numero testado nunca sera zero, pois zero significa que a posicao esta vazia.
      Se o valor testado for igual ao valor contido na posicao ele é valido, pois deveria estar ali mesmo.
    */
    if (getValue(_nLin, _nCol) == _nVal)
        return true;

    /*
      Posicao ja esta preenchida.
      Devido a ter passado pela verificacao anterior, esta prennchida com um numero diferente do que esta
      sendo verificado. Se a posicao ja esta preenchida com um numero diferente de que esta sendo testado ele
      e invalido, pois nao poderia sobrescrever o numero que ja existe na posicao.
    */
    if (getValue(_nLin, _nCol) != 0)
        return false;

    /*
       Se chegou aqui podemos considerar que a posicao testada esta vazia, entao vamos verivicar se vai
       existir algum conflito se atribuindo o valor testado na posicao informada.
    */

    /*
      Coluna possui o valor que esta sendo testado.
      Se a coluna possuir o valor que esta sendo testado para uma posicao, significa que ele e invalido para a
      posicao, pois iria causar replicacao na coluna.
    */
    for (c = 0; c < 9; ++c)
        if (getValue(_nLin, c) == _nVal)
            return false;

    /*
      Linha possui o valor que esta sendo testado.
      Se a linha possuir o valor que esta sendo testado para uma posicao, significa que ele e invalido para a
      posicao, pois iria causar replicacao na linha.
    */
    for (l = 0; l < 9; ++l)
        if (getValue(l, _nCol) == _nVal)
            return false;

    /*
      Testa se vai existir replicação no quadrante.
    */
    const auto b = g_rowCol2BlockNumber[_nLin][_nCol];
    for (int e = 0; e < 9; ++e)
    {
        const auto &rowCol = g_blockElem2RowCol[b][e];
        if (getValue(rowCol.first, rowCol.second) == _nVal)
            return false;
    }
    // lr = _nLin / 3;
    // cr = _nCol / 3;
    // for (l = lr * 3; l < (lr + 1) * 3; ++l)
    //     for (c = cr * 3; c < (cr + 1) * 3; ++c)
    //         if (getValue(l, c) == _nVal)
    //             return false;

    /*
      A posicao esta vazia, e atribuindo o valor testano na posicao informada nao causa nenhuma violacao.
    */
    return true;
}

bool Grid::hasEmptyNoteForNotSetValue()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if ((m_cells[i][j].getValue() == 0) && !m_cells[i][j].hasNote())
            {
                return true;
            }
        }
    }

    return false;
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
                        setNoteVisible(i, j, x, true);
                }
            }
        }
    }
}

void Grid::clearNotes()
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            getCell(i, j).setNotes(0);
}

bool Grid::clearNotesCascade(int _nLin, int _nCol, int _nValue)
{
    int lr = _nLin / 3;
    int cr = _nCol / 3;

    {
        Cell &cell = getCell(_nLin, _nCol);
        cell.clearNotes();
    }

    for (int l = 0; l < 9; ++l)
    {
        if (l == _nLin)
            continue;
        Cell &cell = getCell(l, _nCol);
        if (cell.getNoteVisible(_nValue))
        {
            cell.setNoteVisible(_nValue, false);
            if (!cell.hasNote())
            {
                return false;
            }
        }
    }

    for (int c = 0; c < 9; ++c)
    {
        if (c == _nCol)
            continue;
        Cell &cell = getCell(_nLin, c);
        if (cell.getNoteVisible(_nValue))
        {
            cell.setNoteVisible(_nValue, false);
            if (!cell.hasNote())
            {
                return false;
            }
        }
    }

    for (int l = lr * 3; l < (lr + 1) * 3; ++l)
    {
        for (int c = cr * 3; c < (cr + 1) * 3; ++c)
        {
            if ((l == _nLin) && (c == _nCol))
                continue;
            Cell &cell = getCell(l, c);
            if (cell.getNoteVisible(_nValue))
            {
                cell.setNoteVisible(_nValue, false);
                if (!cell.hasNote())
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void Grid::clearRowNotes(int _row, int _val, const std::function<bool(int)> &_clear)
{
    for (int c = 0; c < 9; ++c)
    {
        if (getNoteVisible(_row, c, _val) && _clear(c))
            setNoteVisible(_row, c, _val, false);
    }
}

void Grid::clearColNotes(int _col, int _val, const std::function<bool(int)> &_clear)
{
    for (int l = 0; l < 9; ++l)
    {
        if (getNoteVisible(l, _col, _val) && _clear(l))
            setNoteVisible(l, _col, _val, false);
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
            if (!_callback(l, c, g_rowCol2BlockNumber[l][c]))
            {
                return;
            }
        }
    }
}