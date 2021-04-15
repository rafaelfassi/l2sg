#include "grid.h"

#include <iostream>

Grid::Grid() : m_dumpCount(0)
{
    m_cells = new Cell[9*9];
}

Grid::Grid(Grid const& other)
{
    int totalSize = 9*9;

    m_cells = new Cell[totalSize];

    for(int x = 0; x < totalSize; x++)
        m_cells[x] = other.m_cells[x];
}

Grid::~Grid()
{
    if(m_cells)
    {
        delete []m_cells;
    }
}

Cell *Grid::getCell(int _nLin, int _nCol)
{
    return &m_cells[ _nCol + (_nLin * 9) ];
}

Cell *Grid::getTranslatedCell(int _i, int _j, int type)
{
    int l, c;
    translateCoordinates(_i, _j, l, c, type);
    return getCell(l, c);
}

void Grid::translateCoordinates(int _i, int _j, int &_l, int &_c, int type)
{
    switch(type)
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
        _l = (_j / 3) + (_i / 3) * 3;
        _c = (_j % 3) + ((_i - (_l / 3) * 3) * 3);
        break;
    }
}

std::pair<int, int> Grid::getBlockStartCoordinates(int _b)
{
    int l = (_b / 3) * 3;
    int c = (_b % 3) * 3;
    return std::make_pair(l, c);
}

int* Grid::getMatrixCopy()
{
    return new int;
}

void Grid::setValues(int *_pValues)
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            setValue(i, j, _pValues[j + (i * 9)]);
}

void Grid::setValues(const std::string &values)
{
    int arr[9*9];
    if(fillArrayFormString(values, arr))
        setValues(arr);
}

bool Grid::fillArrayFormString(const std::string &values, int *array)
{
    int x(0);
    for(size_t i = 0; i < values.size(); ++i)
    {
        const auto ch = values[i];

        if(std::isspace(ch))
            continue;

        if(x == 9*9)
            return false;

        if(std::isdigit(ch))
            array[x++] = (ch - '0'); // Converts the character to int. Notice that '0' == 48
        else
            array[x++] = 0;
    }

    return (x == 9*9);
}

int Grid::getValue(int _nLin, int _nCol) const
{
    return m_cells[ _nCol + (_nLin * 9) ].getValue();
}

void Grid::setValue(int _nLin, int _nCol, int _nVal)
{
    m_cells[ _nCol + (_nLin * 9) ].setValue(_nVal);
}

bool Grid::getNoteVisible(int _nLin, int _nCol, int _nVal)
{
    return m_cells[ _nCol + (_nLin * 9) ].getNoteVisible(_nVal);
}

void Grid::setNoteVisible(int _nLin, int _nCol, int _nVal, bool _bVisible)
{
    m_cells[ _nCol + (_nLin * 9) ].setNoteVisible(_nVal, _bVisible);
}

bool Grid::compareValues(int *_pValues)
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(getValue(i, j) != _pValues[j + (i * 9)])
                return false;
    return true;
}

bool Grid::compareValues(const Grid &_grid)
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(getValue(i, j) != _grid.getValue(i, j))
                return false;
    return true;
}

void Grid::dump(int _dumpFlags, const std::string &_null, const std::string &_numSep)
{
    std::cout << "Dump:" << ++m_dumpCount << std::endl;

    std::cout << "Valores:" << std::endl;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int val = getValue(i, j);
            if(val)
                std::cout << val << _numSep;
            else
                std::cout << _null << _numSep;

            if (!(_dumpFlags & D_ONE_LINE) && j % 3 == 2) std::cout << "  ";
        }
        if(!(_dumpFlags & D_ONE_LINE))
        {
            std::cout << std::endl;
            if (i % 3 == 2) std::cout << std::endl;
        }
    }

    if(_dumpFlags & D_ONE_LINE) std::cout << std::endl;


    if(_dumpFlags & D_ANOTATION)
    {
        std::cout << "Anotacoes:" << std::endl;
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                for(int x = 1; x <= 9; x++)
                {
                    if(getNoteVisible(i, j, x)) std::cout << x;
                    else std::cout << "X";
                }
                std::cout << " ";
                if (j % 3 == 2) std::cout << "  ";
            }
            std::cout << std::endl;
            if (i % 3 == 2) std::cout << std::endl;
        }
    }

}

bool Grid::checkRules(int _nLin, int _nCol, int _nVal)
{
    int l, c, lr, cr;

    /*
      Se a posicao possui o numero que esta sendo testado.
      O numero testado nunca sera zero, pois zero significa que a posicao esta vazia.
      Se o valor testado for igual ao valor contido na posicao ele é valido, pois deveria estar ali mesmo.
    */
    if (getValue(_nLin, _nCol) == _nVal) return true;

    /*
      Posicao ja esta preenchida.
      Devido a ter passado pela verificacao anterior, esta prennchida com um numero diferente do que esta sendo verificado.
      Se a posicao ja esta preenchida com um numero diferente de que esta sendo testado ele e invalido, pois nao poderia sobrescrever
          o numero que ja existe na posicao.
    */
    if (getValue(_nLin, _nCol) != 0) return false;

    /*
       Se chegou aqui podemos considerar que a posicao testada esta vazia, entao vamos verivicar se vai existir algum conflito
        se atribuindo o valor testado na posicao informada.
    */

    /*
      Coluna possui o valor que esta sendo testado.
      Se a coluna possuir o valor que esta sendo testado para uma posicao, significa que ele e invalido para a posicao, pois
       iria causar replicacao na coluna.
    */
    for (c = 0; c < 9; c++)
      if (getValue(_nLin, c) == _nVal) return false;

    /*
      Linha possui o valor que esta sendo testado.
      Se a linha possuir o valor que esta sendo testado para uma posicao, significa que ele e invalido para a posicao, pois
       iria causar replicacao na linha.
    */
    for (l = 0; l < 9; l++)
      if (getValue(l, _nCol) == _nVal) return false;

    /*
      Testa se vai existir replicação no quadrante.
    */
    lr = _nLin / 3;
    cr = _nCol / 3;
    for (l = lr * 3; l < (lr + 1) * 3; l++)
      for (c = cr * 3; c < (cr + 1) * 3; c++)
        if (getValue(l, c) == _nVal) return false;


    /*
      A posicao esta vazia, e atribuindo o valor testano na posicao informada nao causa nenhuma violacao.
    */
    return true;
}

bool Grid::isFull()
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(getValue(i, j) == 0)
                return false;
    return true;
}

void Grid::fillNotes()
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(getValue(i, j) == 0)
            {
                for(int x = 1; x <= 9; x++)
                {
                    if(checkRules(i, j, x)) setNoteVisible(i, j, x, true);
                }
            }
        }
    }
}

void Grid::clearNotes()
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            getCell(i, j)->setNotes(0);
}

void Grid::clearNotesCascade(int _nLin, int _nCol, int _nValue)
{
    int lr = _nLin / 3;
    int cr = _nCol / 3;

    Cell *cell = getCell(_nLin, _nCol);
    cell->clearNotes();

    for(int l = 0; l < 9; l++)
    {
        if(l == _nLin) continue;
        if(getNoteVisible(l, _nCol, _nValue))
            setNoteVisible(l, _nCol, _nValue, false);
    }

    for(int c = 0; c < 9; c++)
    {
        if(c == _nCol) continue;
        if(getNoteVisible(_nLin, c, _nValue))
            setNoteVisible(_nLin, c, _nValue, false);
    }

    for (int l = lr * 3; l < (lr + 1) * 3; l++)
    {
      for (int c = cr * 3; c < (cr + 1) * 3; c++)
      {
          if((l == _nLin) && (c == _nCol)) continue;
          if(getNoteVisible(l, c, _nValue))
              setNoteVisible(l, c, _nValue, false);
      }
    }

}

void Grid::clearRowNotes(int _row, int _val, const std::function<bool(int)> &_clear)
{
    for(int c = 0; c < 9; ++c)
    {
        if(getNoteVisible(_row, c, _val) && _clear(c))
            setNoteVisible(_row, c, _val, false);
    }
}

void Grid::clearColNotes(int _col, int _val, const std::function<bool(int)> &_clear)
{
    for(int l = 0; l < 9; ++l)
    {
        if(getNoteVisible(l, _col, _val) && _clear(l))
            setNoteVisible(l, _col, _val, false);
    }
}

std::string Grid::getNotesSignature()
{
    std::string result;

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            result += std::to_string(getCell(i, j)->getNotes());
        }
    }

    return result;
}
