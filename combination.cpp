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
}
