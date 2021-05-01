#ifndef SUDOKU_COMBINATION_GEN_H
#define SUDOKU_COMBINATION_GEN_H

#include <vector>

namespace sudoku
{
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

} // namespace sudoku

#endif // SUDOKU_COMBINATION_GEN_H
