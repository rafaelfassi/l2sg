#ifndef SUDOKU_UTILS_H
#define SUDOKU_UTILS_H

#include <bitset>

namespace sudoku::utils
{
template <size_t N> int getFirst(const std::bitset<N> &bs)
{
    for (int i = 0; i < bs.size(); ++i)
    {
        if (bs.test(i))
            return i;
    }

    return -1;
}
} // namespace sudoku::utils

#endif // SUDOKU_UTILS_H
