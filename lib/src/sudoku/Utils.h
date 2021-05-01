#ifndef SUDOKU_UTILS_H
#define SUDOKU_UTILS_H

#include <bitset>

namespace sudoku::utils
{

// To get the first found position, pos = -1
// Returns the next found position, or -1 otherwise.
template <size_t N> int getNext(const std::bitset<N> &bs, int pos)
{
    for (int i = pos + 1; i < bs.size(); ++i)
    {
        if (bs.test(i))
            return i;
    }
    return -1;
}

} // namespace sudoku::utils

#endif // SUDOKU_UTILS_H
