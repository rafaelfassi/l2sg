#include "solverlogic.h"
#include "combination.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <optional>
#include <stack>

//Melhorias

//X2XXXXXX9 XXXXXXXXX X2XX567XX   XXXX5XXX9 XXXXXXXXX XXXXXXXXX   X2XX5XXXX XXXXXXXXX XXXXX67XX
//O 6 e 7 só podem estar em 2 campos, então no campo X2XX567XX podemos limpar o resto e deixar somente 6 e 7.
//X2XXXXXX9 XXXXXXXXX XXXXX67XX   XXXX5XXX9 XXXXXXXXX XXXXXXXXX   X2XX5XXXX XXXXXXXXX XXXXX67XX

class Guesses
{
public:
    Guesses(Grid &_pGrid) : m_pGrid(_pGrid), cellCount(2), m_currentCell(0)
    {

    }

    bool validateGuess()
    {
        if (m_savedPoints.size())
        {
            if (!m_pGrid.isValuesValid())
            {
                m_conflicts.insert(m_cellGuesses[m_currentGuessIdx]);
                if (m_conflicts.size() == (m_cellGuesses.size() - 1))
                {
                    for (const auto guess: m_cellGuesses)
                    {
                        if (m_conflicts.find(guess) == m_conflicts.end())
                        {
                            m_pGrid = m_savedPoints.top();
                            const auto cellCoord = m_pGrid.getCellCoordinates(m_currentCell);
                            m_pGrid.setValue(cellCoord.first, cellCoord.second, guess);
                            m_pGrid.clearNotesCascade(cellCoord.first, cellCoord.second, guess);
                            m_savedPoints.pop();
                            m_cellGuesses.clear();
                            break;
                        }
                    }
                }
                return false;
            }
        }

        return true;
    }

    bool nextGuess()
    {
        if (m_savedPoints.empty())
        {
            m_savedPoints.push(m_pGrid);
        }
        else
        {
            m_pGrid = m_savedPoints.top();
        }

        if (m_cellGuesses.size())
        {
            if (++m_currentGuessIdx < m_cellGuesses.size())
            {
                const auto cellCoord = m_pGrid.getCellCoordinates(m_currentCell);
                m_pGrid.setValue(cellCoord.first, cellCoord.second, m_cellGuesses[m_currentGuessIdx]);
                m_pGrid.clearNotesCascade(cellCoord.first, cellCoord.second, m_cellGuesses[m_currentGuessIdx]);
                return true;
            }

            ++m_currentCell;
        }

        for (; m_currentCell < 9*9; ++m_currentCell)
        {
            const auto cellCoord = m_pGrid.getCellCoordinates(m_currentCell);
            auto cell = m_pGrid.getCell(cellCoord.first, cellCoord.second);
            if (cell->notesCount() == cellCount)
            {
                m_currentGuessIdx = 0;
                m_conflicts.clear();
                m_cellGuesses = cell->getVisibleNotesLst();
                m_pGrid.setValue(cellCoord.first, cellCoord.second, m_cellGuesses[m_currentGuessIdx]);
                m_pGrid.clearNotesCascade(cellCoord.first, cellCoord.second, m_cellGuesses[m_currentGuessIdx]);
                return true;
            }
        }

        if (m_currentCell >= 9*9)
        {
            m_currentCell = 0;
            ++cellCount;
            m_cellGuesses.clear();
            return nextGuess();
        }

        return false;
    }

private:
    Grid &m_pGrid;
    std::stack<Grid> m_savedPoints;
    int m_currentCell;
    std::vector<int> m_cellGuesses;
    std::unordered_set<int> m_conflicts;
    int m_currentGuessIdx;
    int cellCount;
};

SolverLogic::SolverLogic(Grid &_pGrid)
    : AbstractSolver(_pGrid), m_level(0)
{
}

void SolverLogic::solve()
{
    std::string notesSignature;
    std::string newNotesSignature;
    int maxLevel(3);
    bool loop(true);
    Guesses guesses(m_pGrid);

    do
    {
        if(newNotesSignature.empty())
            notesSignature = m_pGrid.getNotesSignature();
        else
            notesSignature = newNotesSignature;

        if(m_level > 1)
        {
            solveCandidatesOnlyInBlockLineOrCol();
            solveNumCellsEqualNumCandidates();
        }
        else if(m_level > 0)
        {
            solveNumCellsEqualNumCandidates(2);
        }

        while(solveSolitaryCandidate() > 0);

        while(solveUniquePossibility() > 0);

        if ((m_level <= 2) || guesses.validateGuess())
        {
            if(m_pGrid.isFull())
                break;
        }

        newNotesSignature = m_pGrid.getNotesSignature();

        if(newNotesSignature == notesSignature)
        {
            if (m_level < maxLevel)
            {
                ++m_level;
            }

            if(m_level > 2)
            {
                if (guesses.nextGuess())
                    continue;
            }

            loop = (m_level < maxLevel);
            // if(!loop)
            // {
            //     //m_level = level;
            //     // std::cout << std::endl;
            //     // std::cout << "***************************************************************" << std::endl;
            //     // std::cout << "Level:" << m_level << std::endl;
            //     // std::cout << "***************************************************************" << std::endl;
            //     // std::cout << std::endl;
            // }
        }
        // else
        // {
        //     level = std::max(level - 1, 0);
        // }

    } while(loop);
}

int SolverLogic::solveSolitaryCandidate()
{
    int solvedCount(0);

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
                ++solvedCount;
                // std::cout << "Solitario >> Lin: " << i+1 << " - Col: " << j+1 << " - Num: " << x << std::endl;
                // m_pGrid.dump();
            }
        }
    }

    return solvedCount;
}

int SolverLogic::solveUniquePossibility()
{
    int solvedCount(0);
    int count(0);
    int l1(-1);
    int c1(-1);

    for(int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
    {
        // Para cada numero de 1 a 9
        for(int x=1; x <=9; x++)
        {
            // Percorrer todas as linhas
            for(int i = 0; i < 9; i++)
            {
                count = 0;
                c1 = -1;
                l1 = -1;
                // Percorrer todas as colunas da linha atual procurando pelo numero
                for(int j = 0; j < 9; j++)
                {
                    int l, c;
                    m_pGrid.translateCoordinates(i, j, l, c, type);

                    if(m_pGrid.getNoteVisible(l, c, x))
                    {
                        l1 = l;
                        c1 = c;
                        count++;
                    }
                    // Se o numero estiver em mais de uma coluna nao e a unica possibilidade
                    if(count > 1) break;
                }

                // Se o numero so exite em uma coluna essa e a unica possibilidade
                if(count == 1)
                {
                    m_pGrid.setValue(l1, c1, x);
                    m_pGrid.clearNotesCascade(l1, c1, x);
                    ++solvedCount;
                    // std::cout << "Unica possibilidade para "
                    //           << (type == Grid::T_LINE ? "Linha" : (type == Grid::T_COLUMN ? "Coluna" : "Bloco"))
                    //           << std::endl;
                    // std::cout << "Lin: " << l1+1 << " - Col: " << c1+1 << " - Num: " << x << std::endl;
                    // m_pGrid.dump();
                }
            }
        }
    }

    return solvedCount;
}

void SolverLogic::solveCandidatesOnlyInBlockLineOrCol()
{
    for (int i = 0; i < 9; ++i)
    {
        std::unordered_map<int, std::unordered_set<int>> rows;
        std::unordered_map<int, std::unordered_set<int>> cols;
        for (int j = 0; j < 9; ++j)
        {
            int l, c;
            m_pGrid.translateCoordinates(i, j, l, c, Grid::T_BLOCK);
            Cell *cell = m_pGrid.getCell(l, c);
            auto notes = cell->getVisibleNotesLst();

            rows[l].insert(notes.begin(), notes.end());
            cols[c].insert(notes.begin(), notes.end());
        }

        for (int val = 1; val <= 9; ++val)
        {
            std::optional<int> target_line;
            std::optional<int> target_col;

            for (const auto& [rowNum, row] : rows)
            {
                if (row.find(val) != row.end())
                {
                    if(!target_line.has_value())
                    {
                        target_line = rowNum;
                    }
                    else
                    {
                        target_line.reset();
                        break;
                    }
                }
            }

            for (const auto& [colNum, col] : cols)
            {
                if (col.find(val) != col.end())
                {
                    if(!target_col.has_value())
                    {
                        target_col = colNum;
                    }
                    else
                    {
                        target_col.reset();
                        break;
                    }
                }
            }

            if (target_line.has_value())
            {
                const auto col = m_pGrid.getBlockStartCoordinates(i).second;
                m_pGrid.clearRowNotes(*target_line, val, [col](int c){
                    return (c < col) || (c > col + 2);
                });
            }

            if (target_col.has_value())
            {
                const auto row = m_pGrid.getBlockStartCoordinates(i).first;
                m_pGrid.clearColNotes(*target_col, val, [row](int r){
                    return (r < row) || (r > row + 2);
                });
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
void SolverLogic::solveNumCellsEqualNumCandidates(int maxCells)
{
    Combination combination;

    const int n = 9;
    const int rMax = maxCells ? maxCells : n-1;

    for(int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
    {
        for(int i = 0; i < n; ++i)
        {
            for(int r = 2; r <= rMax; ++r)
            {
                std::vector<int> combLst;
                combination.reset(n, r);

                while(combination.getNextCombination(combLst))
                {
                    int celNotes(0), totalNotes(0);
                    std::unordered_set<int> cels;

                    for(size_t c = 0; c < combLst.size(); ++c)
                    {
                        int j = combLst.at(c);
                        Cell *cell = m_pGrid.getTranslatedCell(i, j, type);
                        celNotes = cell->getNotes();
                        if(celNotes)
                        {
                            totalNotes |= celNotes;
                            cels.insert(j);
                        }
                        else break;
                    }

                    if(celNotes && Cell::notesCount(totalNotes) == cels.size())
                    {
                        // Limpar todas as ocorrencias contidas em "totalNotes" das celulas que não estajam no vetor cels.
                        bool first(true);
                        for(int j = 0; j < 9; ++j)
                        {
                            if(cels.find(j) == cels.end())
                            {
                                int l, c;
                                m_pGrid.translateCoordinates(i, j, l, c, type);
                                Cell *cell = m_pGrid.getCell(l, c);
                                int notes = cell->getNotes();
                                int newNotes = notes & ~totalNotes;

                                if(notes != newNotes)
                                {
                                    cell->setNotes(newNotes);

                                    // if(first)
                                    // {
                                    //     std::cout
                                    //             << "Numero de possibilidades igual ao número de células para "
                                    //             << (type == Grid::T_LINE ? "Linha" : (type == Grid::T_COLUMN ? "Coluna" : "Bloco"))
                                    //             << std::endl;
                                    //     std::cout << "Células:" << std::endl;
                                    //     for (const auto j1 : cels)
                                    //     {
                                    //         int l1, c1;
                                    //         m_pGrid.translateCoordinates(i, j1, l1, c1, type);
                                    //         std::cout  << "Lin: " << l1+1 << " - Col: " << c1+1 << std::endl;
                                    //     }

                                    //     std::cout << "Removendo ocorrencias de ";
                                    //     std::vector<int> lst = Cell::getVisibleNotesLst(totalNotes);
                                    //     for (const auto oc : lst) { std::cout << oc << " "; }
                                    //     std::cout << "nas células:" << std::endl;
                                    // }

                                    // std::cout  << "Lin: " << l+1 << " - Col: " << c+1 << std::endl;
                                    first = false;
                                }
                            }
                        }

                        if(!first)
                        {
                            // std::cout << std::endl;
                            // m_pGrid.dump();
                            return;
                        }
                    }
                }
            }
        }
    }
}
