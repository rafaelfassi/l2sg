#include "solverlogic.h"
#include "combination.h"
#include <cstring>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Melhorias

// X2XXXXXX9 XXXXXXXXX X2XX567XX   XXXX5XXX9 XXXXXXXXX XXXXXXXXX   X2XX5XXXX XXXXXXXXX XXXXX67XX
// O 6 e 7 só podem estar em 2 campos, então no campo X2XX567XX podemos limpar o resto e deixar somente 6 e 7.
// X2XXXXXX9 XXXXXXXXX XXXXX67XX   XXXX5XXX9 XXXXXXXXX XXXXXXXXX   X2XX5XXXX XXXXXXXXX XXXXX67XX

class Guesses
{
public:
    Guesses(Grid &_pGrid) : m_pGrid(_pGrid), m_cellCount(2), m_currentCell(0), m_hasAppliedCandidate(false) {}

    // Verify if one of the cell's candidates is the right value, if so, apply the value permanentely to the
    // grid.
    void tryConfirmCurrentGuess()
    {
        if (m_savedPoint.has_value() && m_cellCandidates.size())
        {
            // The guess has caused a conflict?
            if (m_pGrid.hasEmptyNoteForNotSetValue())
            {
                m_conflicts.insert(m_cellCandidates[m_currentCandidateIdx]);

                // If only one candidate has no conflict detected, the candidate is the correct value.
                if (m_conflicts.size() == (m_cellCandidates.size() - 1))
                {
                    for (const auto candidate : m_cellCandidates)
                    {
                        // Find the unique candidate with no conflict and apply it on the grid.
                        if (m_conflicts.find(candidate) == m_conflicts.end())
                        {
                            // Restore the saved point.
                            m_pGrid = m_savedPoint.value();
                            const auto &cellCoord = m_pGrid.getCellCoordinates(m_currentCell);
                            // Apply the value on the grid.
                            m_pGrid.setValue(cellCoord.first, cellCoord.second, candidate);
                            m_pGrid.clearNotesCascade(cellCoord.first, cellCoord.second, candidate);
                            // Delete the old saved point to make nextGuess() re-create it.
                            m_savedPoint = std::nullopt;
                            m_cellCandidates.clear();
                            m_hasAppliedCandidate = true;
                            std::cout << "Candidate: " << candidate
                                      << " comfirmed for cell: " << cellCoord.first << "," << cellCoord.second
                                      << std::endl;
                            break;
                        }
                    }
                }
            }
        }
    }

    bool nextGuess()
    {
        tryConfirmCurrentGuess();

        if (!m_savedPoint.has_value())
        {
            m_savedPoint = m_pGrid;
        }
        else
        {
            m_pGrid = m_savedPoint.value();
        }

        if (m_cellCandidates.size())
        {
            if (tryApplyNextCellCandidate())
            {
                return true;
            }
            else
            {
                ++m_currentCell;
            }
        }

        for (; m_currentCell < 9 * 9; ++m_currentCell)
        {
            const auto cellCoord = m_pGrid.getCellCoordinates(m_currentCell);
            auto &cell = m_pGrid.getCell(cellCoord.first, cellCoord.second);
            if (cell.notesCount() == m_cellCount)
            {
                m_currentCandidateIdx = -1;
                m_conflicts.clear();
                m_cellCandidates = cell.getVisibleNotesLst();
                const int notes = cell.getNotes();
                const int blk(m_pGrid.getBlockNumber(cellCoord.first, cellCoord.second));
                if (((m_triedGuessesRow.find(std::make_pair(cellCoord.first, notes)) ==
                      m_triedGuessesRow.end()) &&
                     (m_triedGuessesCol.find(std::make_pair(cellCoord.second, notes)) ==
                      m_triedGuessesCol.end()) &&
                     (m_triedGuessesBlk.find(std::make_pair(blk, notes)) == m_triedGuessesBlk.end())))
                {
                    m_triedGuessesRow.insert(std::make_pair(cellCoord.first, notes));
                    m_triedGuessesCol.insert(std::make_pair(cellCoord.second, notes));
                    m_triedGuessesBlk.insert(std::make_pair(blk, notes));

                    if (tryApplyNextCellCandidate())
                    {
                        std::cout << "Start " << m_cellCount
                                  << " candidates guessing for cell: " << cellCoord.first << ","
                                  << cellCoord.second << std::endl;
                        return true;
                    }
                }
            }
        }

        if ((m_currentCell >= 9 * 9) && (m_cellCount < 3))
        {
            m_currentCell = 0;
            if (m_hasAppliedCandidate)
            {
                m_cellCount = 2;
                m_hasAppliedCandidate = false;
            }
            else
            {
                ++m_cellCount;
            }
            m_cellCandidates.clear();
            return nextGuess();
        }

        return false;
    }

    bool tryApplyNextCellCandidate()
    {
        if (m_cellCandidates.size())
        {
            if (++m_currentCandidateIdx < m_cellCandidates.size())
            {
                const auto &cellCoord = m_pGrid.getCellCoordinates(m_currentCell);
                const auto candidate = m_cellCandidates[m_currentCandidateIdx];
                m_pGrid.setValue(cellCoord.first, cellCoord.second, candidate);
                m_pGrid.clearNotesCascade(cellCoord.first, cellCoord.second, candidate);
                return true;
            }
        }
        return false;
    }

private:
    Grid &m_pGrid;
    std::optional<Grid> m_savedPoint;
    int m_currentCell;
    std::vector<int> m_cellCandidates;
    std::unordered_set<int> m_conflicts;
    std::set<std::pair<int, int>> m_triedGuessesRow;
    std::set<std::pair<int, int>> m_triedGuessesCol;
    std::set<std::pair<int, int>> m_triedGuessesBlk;
    int m_currentCandidateIdx;
    int m_cellCount;
    bool m_hasAppliedCandidate;
};

SolverLogic::SolverLogic(Grid &_pGrid) : AbstractSolver(_pGrid), m_level(LEV_EASY)
{
}

void SolverLogic::solve()
{
    std::string notesSignature;
    std::string newNotesSignature;
    int maxLevel(LEV_GUESS);
    bool loop(true);
    Guesses guesses(m_pGrid);

    do
    {
        if (newNotesSignature.empty())
            notesSignature = m_pGrid.getNotesSignature();
        else
            notesSignature = newNotesSignature;

        if (m_level >= LEV_HARD)
        {
            solveCandidatesOnlyInBlockLineOrCol(m_pGrid);
            solveNumCellsEqualNumCandidates(m_pGrid);
        }
        else if (m_level >= LEV_MEDIUM)
        {
            solveNumCellsEqualNumCandidates(m_pGrid, 2);
        }

        while (solveSolitaryCandidate(m_pGrid) > 0)
            ;

        while (solveUniquePossibility(m_pGrid) > 0)
            ;

        if (m_pGrid.isFull())
            break;

        newNotesSignature = m_pGrid.getNotesSignature();

        if (newNotesSignature == notesSignature)
        {
            if (m_level < maxLevel)
            {
                ++m_level;
            }

            if (m_level == LEV_GUESS)
            {
                solveBruteForce();
                loop = false;
                // loop = guesses.nextGuess();
            }
            else
            {
                loop = (m_level < maxLevel);
            }
        }

    } while (loop);
}

int SolverLogic::solveSolitaryCandidate(Grid &pGrid, bool *ok)
{
    int solvedCount(0);

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            auto &cell = pGrid.getCell(i, j);
            if (cell.notesCount() == 1)
            {
                int x(0);
                while (!cell.getNoteVisible(x))
                    ++x;
                if (ok)
                {
                    if (!pGrid.isAllowedValue(i, j, x))
                    {
                        *ok = false;
                        return 0;
                    }
                }
                cell.setValue(x);
                pGrid.clearNotesCascade(i, j, x);
                ++solvedCount;
                // std::cout << "Solitario >> Lin: " << i+1 << " - Col: " << j+1 << " - Num: " << x <<
                // std::endl; pGrid.dump();
            }
        }
    }

    if (ok)
    {
        *ok = true;
    }
    return solvedCount;
}

int SolverLogic::solveUniquePossibility(Grid &pGrid, bool *ok)
{
    int solvedCount(0);
    int rows[9][9];
    int cols[9][9];
    std::pair<int, int> blks[9][9];
    memset(rows, -1, sizeof(rows));
    memset(cols, -1, sizeof(cols));
    std::fill(&blks[0][0], &blks[0][0] + 9 * 9, std::make_pair(0, -1));

    // for (int i = 0; i < 9; ++i)
    // {
    //     for (int j = 0; j < 9; ++j)
    //     {
    //         if ((blks[i][j].second != -1) || (rows[i][j] != -1) || (cols[i][j] != -1))
    //         {
    //             std::cout << "ERROR" << std::endl;
    //         }
    //     }
    // }

    // pGrid.dump();

    const std::function<bool(int, int, int)> fillData = [&](int l, int c, int b) -> bool {
        Cell &cell = pGrid.getCell(l, c);
        for (const auto note : cell.getVisibleNotesLst())
        {
            const int noteIdx = note - 1;
            {
                switch (rows[l][noteIdx])
                {
                    case -1:
                        rows[l][noteIdx] = c;
                        break;
                    case -2:
                        break;
                    default:
                        rows[l][noteIdx] = -2;
                }
            }
            {
                switch (cols[c][noteIdx])
                {
                    case -1:
                        cols[c][noteIdx] = l;
                        break;
                    case -2:
                        break;
                    default:
                        cols[c][noteIdx] = -2;
                }
            }
            {
                switch (blks[b][noteIdx].second)
                {
                    case -1:
                        blks[b][noteIdx] = std::make_pair(l, c);
                        break;
                    case -2:
                        break;
                    default:
                        blks[b][noteIdx] = std::make_pair(0, -2);
                }
            }
        }

        return true;
    };

    pGrid.forAllTypes(fillData);

    for (int i = 0; i < 9; ++i)
    {
        for (int v = 1; v < 10; ++v)
        {
            const int vIdx = v - 1;
            if (rows[i][vIdx] > -1)
            {
                const auto c = rows[i][vIdx];
                auto &cell = pGrid.getCell(i, c);
                if (cell.getNotes() != 0)
                {
                    if (ok)
                    {
                        if (!pGrid.isAllowedValue(i, c, v))
                        {
                            *ok = false;
                            return 0;
                        }
                    }
                    ++solvedCount;
                    cell.setValue(v);
                    pGrid.clearNotesCascade(i, c, v);
                }
            }

            if (cols[i][vIdx] > -1)
            {
                const auto l = cols[i][vIdx];
                auto &cell = pGrid.getCell(l, i);
                if (cell.getNotes() != 0)
                {
                    if (ok)
                    {
                        if (!pGrid.isAllowedValue(l, i, v))
                        {
                            *ok = false;
                            return 0;
                        }
                    }
                    ++solvedCount;
                    cell.setValue(v);
                    pGrid.clearNotesCascade(l, i, v);
                }
            }

            if (blks[i][vIdx].second > -1)
            {
                const auto lc = blks[i][vIdx];
                auto &cell = pGrid.getCell(lc.first, lc.second);
                if (cell.getNotes() != 0)
                {
                    if (ok)
                    {
                        if (!pGrid.isAllowedValue(lc.first, lc.second, v))
                        {
                            *ok = false;
                            return 0;
                        }
                    }
                    ++solvedCount;
                    cell.setValue(v);
                    pGrid.clearNotesCascade(lc.first, lc.second, v);
                }
            }
        }
    }

    if (ok)
    {
        *ok = true;
    }

    return solvedCount;
}

int SolverLogic::solveUniquePossibility_1(Grid &pGrid)
{
    int solvedCount(0);
    std::map<std::pair<int, int>, int> rows;
    std::map<std::pair<int, int>, int> cols;
    std::map<std::pair<int, int>, std::pair<int, int>> blks;

    // pGrid.dump();

    const std::function<bool(int, int, int)> processUnPoss = [&](int l, int c, int b) -> bool {
        Cell &cell = pGrid.getCell(l, c);
        for (const auto note : cell.getVisibleNotesLst())
        {
            {
                const auto &p = rows.insert(std::make_pair(std::make_pair(l, note), c));
                if (!p.second && p.first->second != -1)
                {
                    p.first->second = -1;
                }
            }
            {
                const auto &p = cols.insert(std::make_pair(std::make_pair(c, note), l));
                if (!p.second && p.first->second != -1)
                {
                    p.first->second = -1;
                }
            }
            {
                const auto &p = blks.insert(std::make_pair(std::make_pair(b, note), std::make_pair(l, c)));
                if (!p.second && p.first->second.second != -1)
                {
                    p.first->second.second = -1;
                }
            }
        }

        return true;
    };

    pGrid.forAllTypes(processUnPoss);

    for (const auto &row : rows)
    {
        if (row.second != -1)
        {
            auto &c = pGrid.getCell(row.first.first, row.second);
            if (c.getNotes() != 0 && c.getValue() == 0)
            {
                ++solvedCount;
                c.setValue(row.first.second);
                pGrid.clearNotesCascade(row.first.first, row.second, row.first.second);
            }
        }
    }

    for (const auto &col : cols)
    {
        if (col.second != -1)
        {
            auto &c = pGrid.getCell(col.second, col.first.first);
            if (c.getNotes() != 0 && c.getValue() == 0)
            {
                ++solvedCount;
                c.setValue(col.first.second);
                pGrid.clearNotesCascade(col.second, col.first.first, col.first.second);
            }
        }
    }

    for (const auto &blk : blks)
    {
        if (blk.second.second != -1)
        {
            auto &c = pGrid.getCell(blk.second.first, blk.second.second);
            if (c.getNotes() != 0 && c.getValue() == 0)
            {
                ++solvedCount;
                c.setValue(blk.first.second);
                pGrid.clearNotesCascade(blk.second.first, blk.second.second, blk.first.second);
            }
        }
    }

    return solvedCount;
}

int SolverLogic::solveUniquePossibilityOld(Grid &pGrid)
{
    int solvedCount(0);
    int count(0);
    int l1(-1);
    int c1(-1);

    for (int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
    {
        // Para cada numero de 1 a 9
        for (int x = 1; x <= 9; ++x)
        {
            // Percorrer todas as linhas
            for (int i = 0; i < 9; ++i)
            {
                count = 0;
                c1 = -1;
                l1 = -1;
                // Percorrer todas as colunas da linha atual procurando pelo numero
                for (int j = 0; j < 9; ++j)
                {
                    int l, c;
                    pGrid.translateCoordinates(i, j, l, c, type);

                    if (pGrid.getNoteVisible(l, c, x))
                    {
                        l1 = l;
                        c1 = c;
                        ++count;
                    }
                    // Se o numero estiver em mais de uma coluna nao e a unica possibilidade
                    if (count > 1)
                        break;
                }

                // Se o numero so exite em uma coluna essa e a unica possibilidade
                if (count == 1)
                {
                    pGrid.setValue(l1, c1, x);
                    pGrid.clearNotesCascade(l1, c1, x);
                    ++solvedCount;
                    // std::cout << "Unica possibilidade para "
                    //           << (type == Grid::T_LINE ? "Linha" : (type == Grid::T_COLUMN ? "Coluna" :
                    //           "Bloco"))
                    //           << std::endl;
                    // std::cout << "Lin: " << l1+1 << " - Col: " << c1+1 << " - Num: " << x << std::endl;
                    // pGrid.dump();
                }
            }
        }
    }

    return solvedCount;
}

void SolverLogic::solveCandidatesOnlyInBlockLineOrCol(Grid &pGrid)
{
    for (int i = 0; i < 9; ++i)
    {
        std::unordered_map<int, std::unordered_set<int>> rows;
        std::unordered_map<int, std::unordered_set<int>> cols;
        for (int j = 0; j < 9; ++j)
        {
            int l, c;
            pGrid.translateCoordinates(i, j, l, c, Grid::T_BLOCK);
            Cell &cell = pGrid.getCell(l, c);
            auto notes = cell.getVisibleNotesLst();

            rows[l].insert(notes.begin(), notes.end());
            cols[c].insert(notes.begin(), notes.end());
        }

        for (int val = 1; val <= 9; ++val)
        {
            std::optional<int> target_line;
            std::optional<int> target_col;

            for (const auto &[rowNum, row] : rows)
            {
                if (row.find(val) != row.end())
                {
                    if (!target_line.has_value())
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

            for (const auto &[colNum, col] : cols)
            {
                if (col.find(val) != col.end())
                {
                    if (!target_col.has_value())
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
                const auto col = pGrid.getBlockStartCoordinates(i).second;
                pGrid.clearRowNotes(*target_line, val, [col](int c) { return (c < col) || (c > col + 2); });
            }

            if (target_col.has_value())
            {
                const auto row = pGrid.getBlockStartCoordinates(i).first;
                pGrid.clearColNotes(*target_col, val, [row](int r) { return (r < row) || (r > row + 2); });
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
void SolverLogic::solveNumCellsEqualNumCandidates(Grid &pGrid, int maxCells)
{
    Combination combination;

    const int n = 9;
    const int maxCellsComb = maxCells ? maxCells : n - 1;

    for (int type = Grid::T_LINE; type <= Grid::T_BLOCK; ++type)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int nCellsComb = 2; nCellsComb <= maxCellsComb; ++nCellsComb)
            {
                std::vector<int> combLst;
                combination.reset(n, nCellsComb);

                while (combination.getNextCombination(combLst))
                {
                    int cellNotes(0), totalNotes(0);
                    std::unordered_set<int> cells;

                    for (size_t c = 0; c < combLst.size(); ++c)
                    {
                        int j = combLst.at(c);
                        Cell &cell = pGrid.getTranslatedCell(i, j, type);
                        cellNotes = cell.getNotes();
                        if (cellNotes)
                        {
                            totalNotes |= cellNotes;
                            cells.insert(j);
                        }
                        else
                            break;
                    }

                    if (cellNotes && Cell::notesCount(totalNotes) == cells.size())
                    {
                        // Limpar todas as ocorrencias contidas em "totalNotes" das celulas que não estajam no
                        // vetor cells.
                        bool first(true);
                        for (int j = 0; j < 9; ++j)
                        {
                            if (cells.find(j) == cells.end())
                            {
                                int l, c;
                                pGrid.translateCoordinates(i, j, l, c, type);
                                Cell &cell = pGrid.getCell(l, c);
                                int notes = cell.getNotes();
                                int newNotes = notes & ~totalNotes;

                                if (notes != newNotes)
                                {
                                    cell.setNotes(newNotes);

                                    // if(first)
                                    // {
                                    //     std::cout
                                    //             << "Numero de possibilidades igual ao número de células
                                    //             para "
                                    //             << (type == Grid::T_LINE ? "Linha" : (type ==
                                    //             Grid::T_COLUMN ? "Coluna" : "Bloco"))
                                    //             << std::endl;
                                    //     std::cout << "Células:" << std::endl;
                                    //     for (const auto j1 : cells)
                                    //     {
                                    //         int l1, c1;
                                    //         pGrid.translateCoordinates(i, j1, l1, c1, type);
                                    //         std::cout  << "Lin: " << l1+1 << " - Col: " << c1+1 <<
                                    //         std::endl;
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

                        if (!first)
                        {
                            // std::cout << std::endl;
                            // pGrid.dump();
                            return;
                        }
                    }
                }
            }
        }
    }
}

int SolverLogic::solveBruteForce(int maxSolution)
{
    std::vector<Grid> solutions;
    Grid grid = m_pGrid;

    const std::function<void(int, int)> resolve = [&](const int lin, const int col) {
        if (solutions.size() >= maxSolution)
            return;

        if (lin != 9)
        {
            const auto &cell = grid.getCell(lin, col);

            if (cell.getValue() > 0)
            {
                if (col < 8)
                    resolve(lin, col + 1);
                else
                    resolve(lin + 1, 0);
            }
            else
            {
                const auto candidates = cell.getVisibleNotesLst();
                for (const int condidate : candidates)
                {
                    Grid savedPoint = grid;
                    grid.setValue(lin, col, condidate);
                    if (grid.clearNotesCascade(lin, col, condidate))
                    {
                        bool ok(true);
                        while (solveSolitaryCandidate(grid, &ok) > 0)
                            ;
                        if (ok)
                        {
                            while (solveUniquePossibility(grid, &ok) > 0)
                                ;
                            if (ok)
                            {
                                if (!grid.hasEmptyNoteForNotSetValue())
                                {
                                    if (col < 8)
                                        resolve(lin, col + 1);
                                    else
                                        resolve(lin + 1, 0);
                                }
                            }
                        }
                    }

                    // Se chegar aqui e porque em algum ponto das chamadas recursivas, a funcao
                    // isAllowedValue() retornou false Vamos voltar ao valor anterior e deixar laco for tentar
                    // com outro valor.
                    grid = savedPoint;
                }
            }
        }
        else
        {
            solutions.push_back(grid);
        }
    };

    resolve(0, 0);

    if (!solutions.empty())
    {
        m_pGrid = solutions.front();
    }

    return solutions.size();
}