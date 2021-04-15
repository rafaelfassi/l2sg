#include "solverbruteforce.h"

SolverBruteForce::SolverBruteForce(Grid &_pGrid) : AbstractSolver(_pGrid)
{
}

void SolverBruteForce::solve()
{
    solveSolutions();
    if(m_lSolutions.size())
        m_pGrid = m_lSolutions.at(0);
}

std::vector<Grid> &SolverBruteForce::solveSolutions(size_t _nMaxSolutions)
{
    m_nMaxSolutions = _nMaxSolutions;

    m_lSolutions.clear();
    resolve(0, 0);
    return m_lSolutions;
}

void SolverBruteForce::resolve(int lin, int col)
{
    if(m_lSolutions.size() >= m_nMaxSolutions) return;

    if(lin != 9)
    {
        // Tenta todas as possibilidades para linha e coluna informada
        for(int n = 1; n <= 9; n++)
        {
            // Se atribuir n a posicao atual, ira violar alguma regra?
            if (m_pGrid.checkRules(lin, col, n))
            {
                int t = m_pGrid.getValue(lin, col);
                m_pGrid.setValue(lin, col, n);

                // Tentamos resolver todo o jogo considerando n na posicao atual
                if(col < 8) resolve(lin, col + 1);
                else resolve(lin + 1, 0);

                // Se chegar aqui e porque em algum ponto das chamadas recursivas, a funcao checkRules() retornou false
                // Vamos voltar ao valor anterior e deixar laco for tentar com outro valor.
                m_pGrid.setValue(lin, col, t);
            }
        }
    }
    else
    {
        m_lSolutions.push_back(m_pGrid);
    }
}
