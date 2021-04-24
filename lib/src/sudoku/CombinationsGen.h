#ifndef COMBINATION_GEN_H
#define COMBINATION_GEN_H

#include <vector>

class CombinationsGen
{
public:
    CombinationsGen(int n = 0, int r = 0);
    ~CombinationsGen() = default;
    void reset(int n, int r);
    bool getNextCombination(std::vector<int> &combination);

private:
    std::vector<bool> m_v;
    int m_n;
    int m_r;
    bool m_hasNext;
};

#endif // COMBINATION_GEN_H
