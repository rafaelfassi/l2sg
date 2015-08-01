#ifndef COMBINATION_H
#define COMBINATION_H

#include <vector>
#include <QList>

class Combination
{
public:
    Combination(int n = 0, int r = 0);
    ~Combination();
    void reset(int n, int r);
    bool getNextCombination(QList<int> &combination);

private:
    std::vector<bool> m_v;
    int m_n;
    int m_r;
    bool m_hasNext;
};

#endif // COMBINATION_H
