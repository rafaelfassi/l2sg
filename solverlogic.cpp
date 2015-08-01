#include "solverlogic.h"
#include "combination.h"
#include <iostream>
#include <QList>
#include <QString>

//Melhorias

//X2XXXXXX9 XXXXXXXXX X2XX567XX   XXXX5XXX9 XXXXXXXXX XXXXXXXXX   X2XX5XXXX XXXXXXXXX XXXXX67XX
//O 6 e 7 só podem estar em 2 campos, então no campo X2XX567XX podemos limpar o resto e deixar somente 6 e 7.
//X2XXXXXX9 XXXXXXXXX XXXXX67XX   XXXX5XXX9 XXXXXXXXX XXXXXXXXX   X2XX5XXXX XXXXXXXXX XXXXX67XX

SolverLogic::SolverLogic(Grid &_pGrid) : m_pGrid(_pGrid)
{
}

void SolverLogic::solve()
{
    QString notesSignature;
    QString newNotesSignature;
    int level(0), maxLevel(2);
    bool loop(true);

    do
    {
        if(newNotesSignature.isEmpty()) notesSignature = m_pGrid.getNotesSignature();
        else notesSignature = newNotesSignature;

        while(existNoteSolitary())
        {
            noteSolitarySolve();
        }

        uniquePossibilitySolve();

        if(level > 0) notesReplicatedSolve();

        if(level > 1) numCellQqNumPossibSolve();

        newNotesSignature = m_pGrid.getNotesSignature();

        if(newNotesSignature == notesSignature)
        {
            loop = (++level <= maxLevel);
            if(loop)
            {
                std::cout << std::endl;
                std::cout << "***************************************************************" << std::endl;
                std::cout << "Level:" << level << std::endl;
                std::cout << "***************************************************************" << std::endl;
                std::cout << std::endl;
            }
        }

    } while(loop);

}

bool SolverLogic::existNoteSolitary()
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(m_pGrid.getCell(i, j)->notesCount() == 1) return true;
        }
    }

    return false;
}

void SolverLogic::noteSolitarySolve()
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            Cell *cell = m_pGrid.getCell(i, j);
            if(cell->notesCount() == 1)
            {
                int x(0);
                while(!cell->getNoteVisible(x)) x++;
                cell->setValue(x);
                m_pGrid.clearNotesCascade(i, j, x);
                std::cout << "Solitario >> Lin: " << i+1 << " - Col: " << j+1 << " - Num: " << x << std::endl;
                m_pGrid.dump();
            }
        }
    }
}

void SolverLogic::uniquePossibilitySolve()
{

    int count(0);
    int item(-1);

    // Para cada numero de 1 a 9
    for(int x=1; x <=9; x++)
    {
        // Percorrer todas as linhas
        for(int i = 0; i < 9; i++)
        {
            count = 0;
            item = -1;
            // Percorrer todas as colunas da linha atual procurando pelo numero
            for(int j = 0; j < 9; j++)
            {
                if(m_pGrid.getNoteVisible(i, j, x))
                {
                    item = j;
                    count++;
                }
                // Se o numero estiver em mais de uma coluna nao e a unica possibilidade
                if(count > 1) break;
            }

            // Se o numero so exite em uma coluna essa e a unica possibilidade
            if(count == 1)
            {
                m_pGrid.setValue(i, item, x);
                m_pGrid.clearNotesCascade(i, item, x);
                std::cout << "Unica possibilidade Linha >> Lin: " << i+1 << " - Col: " << item+1 << " - Num: " << x << std::endl;
                m_pGrid.dump();
            }
        }



        for(int j = 0; j < 9; j++)
        {
            count = 0;
            item = -1;
            for(int i = 0; i < 9; i++)
            {
                if(m_pGrid.getNoteVisible(i, j, x))
                {
                    item = i;
                    count++;
                }
                if(count > 1) break;
            }
            if(count == 1)
            {
                m_pGrid.setValue(item, j, x);
                m_pGrid.clearNotesCascade(item, j, x);
                std::cout << "Unica possibilidade Coluna >> Lin: " << item+1 << " - Col: " << j+1 << " - Num: " << x << std::endl;
                m_pGrid.dump();
            }
        }




    }
}


void SolverLogic::notesReplicatedSolve()
{

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            Cell *cell = m_pGrid.getCell(i, j);
            if(cell->notesCount() == 2)
            {
                int notesAt = cell->getNotes();
                //if(i == 8) continue;
                for(int j2 = j+1; j2 < 9; j2++)
                {
                    if(m_pGrid.getCell(i, j2)->getNotes() == notesAt)
                    {
                        clearLinNotesExceptEqual(i, cell->getNotes());
                        //clearBlockNotesExceptEqual(i, j2, notesAt);
                        std::cout << "Nota replicada em linha >> Lin: " << i+1 << " - Col: " << j2+1 << std::endl;
                        m_pGrid.dump();
                    }
                }
            }
        }
    }


    for(int j = 0; j < 9; j++)
    {
        for(int i = 0; i < 9; i++)
        {
            Cell *cell = m_pGrid.getCell(i, j);
            if(cell->notesCount() == 2)
            {
                int notesAt = cell->getNotes();
                //if(i == 8) continue;
                for(int i2 = i+1; i2 < 9; i2++)
                {
                    if(m_pGrid.getCell(i2, j)->getNotes() == notesAt)
                    {
                        clearColNotesExceptEqual(j, notesAt);
                        //clearBlockNotesExceptEqual(i2, j, notesAt);
                        std::cout << "Nota replicada em coluna >> Lin: " << i2+1 << " - Col: " << j+1 << std::endl;
                        m_pGrid.dump();
                    }
                }
            }
        }
    }



    for(int b = 0; b < 9; b++)
    {
        int lIni, cIni;
        getFirstBlockPos(b, lIni, cIni);

        for(int l = lIni; l < (lIni+3); l++)
        {
            for(int c = cIni; c < (cIni+3); c++)
            {

                Cell *cell = m_pGrid.getCell(l, c);
                if(cell->notesCount() == 2)
                {
                    int notesAt = cell->getNotes();

                    int lIni2;
                    int cIni2;

                    if(c == cIni+2)
                    {
                        lIni2 = l+1;
                        cIni2 = cIni;
                    }
                    else
                    {
                        lIni2 = l;
                        cIni2 = c+1;
                    }

                    for(int l2 = lIni2; l2 < (lIni+3); l2++)
                    {
                        for(int c2 = cIni2; c2 < (cIni+3); c2++)
                        {

                            if(m_pGrid.getCell(l2, c2)->getNotes() == notesAt)
                            {
                                clearBlockNotesExceptEqual(l2, c2, notesAt);
                                std::cout << "Nota replicada em bloco >> Lin: " << l2+1 << " - Col: " << c2+1 << std::endl;
                                m_pGrid.dump();
                            }

                        }
                        cIni2 = cIni;
                    }

                }



            }
        }
    }

}

/*
    Sendo p o número total de possibilidades diferentes em n células, no caso de p = n podemos remover essas
    possibilidades das celulas que não pertencem a n.
    Domínio: (Linha ou Coluna ou Bloco)
    Ex:
        XXXX5X78X  XXXXXXXXX  XXXX5X789  XXXXXXXXX  1XX4XXXXX  XXX4X6XXX  XXXX5XX89  1XXX56XXX  1XXX56XXX
        Neste caso os números 1, 4, 5 e 6 são todas as possibilidades para as células 5, 6, 8 e 9.
        Então:
        p = count(1,4,5,6) = 4
        n = count(5,6,8,9) = 4
        Podemos remover as possibilidades 1,4,5 e 6 de todas as células diferentes de 5,6,8 e 9.
        XXXXXX78X  XXXXXXXXX  XXXXXX789  XXXXXXXXX  1XX4XXXXX  XXX4X6XXX  XXXXXXX89  1XXX56XXX  1XXX56XXX

*/
void SolverLogic::numCellQqNumPossibSolve()
{
    Combination combination;

    const int n = 9;

    for(int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
    {
        for(int i = 0; i < n; ++i)
        {
            for(int r = 2; r < n-1; ++r)
            {
                QList<int> combLst;
                combination.reset(n, r);

                while(combination.getNextCombination(combLst))
                {
                    int celNotes(0), totalNotes(0);
                    QList<int> cels;

                    for(int c = 0; c < combLst.size(); ++c)
                    {
                        int j = combLst.at(c);
                        Cell *cell = m_pGrid.getTranslatedCell(i, j, type);
                        celNotes = cell->getNotes();
                        if(celNotes)
                        {
                            totalNotes |= celNotes;
                                cels.push_back(j);
                        }
                        else break;
                    }

                    if(celNotes && Cell::notesCount(totalNotes) == cels.size())
                    {
                        // Limpar todas as ocorrencias contidas em "totalNotes" das celulas que não estajam no vetor cels.
                        bool first(true);
                        for(int j = 0; j < 9; ++j)
                        {
                            if(!cels.contains(j))
                            {
                                int l, c;
                                m_pGrid.translateCoordinates(i, j, l, c, type);
                                Cell *cell = m_pGrid.getCell(l, c);
                                int notes = cell->getNotes();
                                int newNotes = notes & ~totalNotes;

                                if(notes != newNotes)
                                {
                                    cell->setNotes(newNotes);

                                    if(first)
                                    {
                                        std::cout
                                                << "Número de possibilidades igual ao número de células para "
                                                << (type == Grid::T_LINE ? "Linha" : (type == Grid::T_COLUMN ? "Coluna" : "Bloco"))
                                                << std::endl;
                                        std::cout << "Células:" << std::endl;
                                        foreach (int j1, cels)
                                        {
                                            int l1, c1;
                                            m_pGrid.translateCoordinates(i, j1, l1, c1, type);
                                            std::cout  << "Lin: " << l1+1 << " - Col: " << c1+1 << std::endl;
                                        }

                                        std::cout << "Removendo ocorrencias de ";
                                        QList<int> lst = Cell::getVisibleNotesLst(totalNotes);
                                        foreach (int oc, lst) { std::cout << oc << " "; }
                                        std::cout << "nas células:" << std::endl;
                                    }

                                    std::cout  << "Lin: " << l+1 << " - Col: " << c+1 << std::endl;
                                    first = false;
                                }
                            }
                        }

                        if(!first)
                        {
                            std::cout << std::endl;
                            m_pGrid.dump();
                        }
                    }
                }
            }
        }
    }
}

void SolverLogic::limparOcorrExterna(QList<int> &cels, int ocorrencias)
{
    for(int i = 0; i < 9; ++i)
    {
        if(!cels.contains(i))
        {
            Cell *cell = m_pGrid.getCell(3, i);
            int notes = cell->getNotes();
            int newNotes = notes & ~ocorrencias;

            if(notes != newNotes)
            {
                cell->setNotes(cell->getNotes() & ~ocorrencias);
                std::cout << "numCellQqNumPossib >> Lin: " << 3+1 << " - Col: " << i+1 << std::endl;
                m_pGrid.dump();
            }
        }
    }
}

void SolverLogic::clearLinNotesExceptEqual(int _nLin, int _nNotes)
{
    QList<int> lst = Cell::getVisibleNotesLst(_nNotes);

    for(int c = 0; c < 9; c++)
    {
        Cell *cell = m_pGrid.getCell(_nLin, c);
        if(cell->getNotes() != _nNotes)
        {
            foreach (const int &noteNum, lst)
            {
                m_pGrid.setNoteVisible(_nLin, c, noteNum, false);
            }
        }
    }
}

void SolverLogic::clearColNotesExceptEqual(int _nCol, int _nNotes)
{
    QList<int> lst = Cell::getVisibleNotesLst(_nNotes);

    for(int l = 0; l < 9; l++)
    {
        Cell *cell = m_pGrid.getCell(l, _nCol);
        if(cell->getNotes() != _nNotes)
        {
            foreach (const int &noteNum, lst)
            {
                m_pGrid.setNoteVisible(l, _nCol, noteNum, false);
            }
        }
    }
}

void SolverLogic::clearBlockNotesExceptEqual(int _nLin, int _nCol, int _nNotes)
{
    int lr = _nLin / 3;
    int cr = _nCol / 3;

    QList<int> lst = Cell::getVisibleNotesLst(_nNotes);

    for (int l = lr * 3; l < (lr + 1) * 3; l++)
    {
        for (int c = cr * 3; c < (cr + 1) * 3; c++)
        {
            Cell *cell = m_pGrid.getCell(l, c);
            if(cell->getNotes() != _nNotes)
            {
                foreach (const int &noteNum, lst)
                {
                    m_pGrid.setNoteVisible(l, c, noteNum, false);
                }
            }
        }
    }
}

void SolverLogic::getFirstBlockPos(int _nBlockNum, int &_nLin, int &_nCol)
{
    _nLin = (_nBlockNum / 3) * 3;
    _nCol = (_nBlockNum - _nLin) * 3;
}
