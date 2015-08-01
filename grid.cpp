#include "grid.h"

#include <iostream>
#include <QString>

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

int *Grid::getMatrixCopy()
{
    return new int;
}

void Grid::setValues(int *_pValues)
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            setValue(i, j, _pValues[j + (i * 9)]);
}

int Grid::getValue(int _nLin, int _nCol)
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

void Grid::dump()
{
    std::cout << "Dump:" << ++m_dumpCount << std::endl;

    std::cout << "Valores:" << std::endl;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            std::cout << getValue(i, j) << " ";
            if (j % 3 == 2) std::cout << "  ";
        }
        std::cout << std::endl;
        if (i % 3 == 2) std::cout << std::endl;
    }


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

QString Grid::getNotesSignature()
{
    QString result;

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            result += QString::number(getCell(i, j)->getNotes());
        }
    }

    return result;
}
