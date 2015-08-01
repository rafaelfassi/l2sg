#include "combination.h"
#include <algorithm>

Combination::Combination(int n, int r)
    : m_hasNext(true)
{
    if(n && r)
        reset(n, r);
}

Combination::~Combination()
{

}

void Combination::reset(int n, int r)
{
    m_n = n;
    m_r = r;
    m_hasNext = true;
    m_v.clear();
    m_v.resize(m_n);

    std::fill(m_v.begin() + m_r, m_v.end(), true);
}

bool Combination::getNextCombination(QList<int> &combination)
{
    combination.clear();

    if(!m_hasNext)
        return false;

    for (int i = 0; i < m_n; ++i)
    {
        if (!m_v[i])
        {
            combination.append(i);
        }
    }

    m_hasNext = std::next_permutation(m_v.begin(), m_v.end());

    return true;

/*
    do
    {
        std::vector<int> ocorrencias;
        std::vector<int> cels;
        bool valid(false);

        for (int i = 0; i < n; ++i)
        {
            if (!v[i])
            {
                Celula cel = getCel(i);
                valid = (getOcorrencias(cel, ocorrencias) > 0);
                if(valid)
                    cels.push_back(i);
                else
                    break;
            }
        }

        if(valid && ocorrencias.size() == cels.size())
        {
            // Limpar todas as ocorrencias contidas no vetor "ocorrencias" das celulas que não estajam no vetor cels.
            limparOcorrExterna(cels, ocorrencias);
        }

    } while (std::next_permutation(v.begin(), v.end()));
*/

}
