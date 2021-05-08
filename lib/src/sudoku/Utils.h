#ifndef SUDOKU_UTILS_H
#define SUDOKU_UTILS_H

#include <bitset>

namespace sudoku::utils
{

class CombinationsGen
{
public:
    CombinationsGen(int n = 0, int r = 0)
    {
        if (n && r)
            reset(n, r);
    }
    ~CombinationsGen() = default;
    void reset(int n, int r)
    {
        m_n = n;
        m_r = r;
        m_hasNext = true;
        m_v.clear();
        m_v.resize(m_n);
        std::fill(m_v.begin() + m_r, m_v.end(), true);
    }
    bool getNextCombination(std::vector<int> &combination)
    {
        combination.clear();
        if (!m_hasNext)
            return false;
        for (int i = 0; i < m_n; ++i)
        {
            if (!m_v[i])
            {
                combination.push_back(i);
            }
        }
        m_hasNext = std::next_permutation(m_v.begin(), m_v.end());
        return true;
    }

private:
    std::vector<bool> m_v;
    int m_n;
    int m_r;
    bool m_hasNext;
};

template <std::size_t N, typename BS> class bitset_iterator
{
public:
    class const_iterator
    {
    public:
        explicit const_iterator(const std::bitset<N> &bitset)
            : index{static_cast<std::size_t>(-1)}, m_bs(bitset)
        {
        }

        const_iterator operator++()
        {
            this->seek_next();
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator prev_this = *this;
            this->seek_next();
            return prev_this;
        }

        const std::size_t &operator*() const { return this->index; }

        bool operator==(const const_iterator &rhs) const
        {
            return (this->index == rhs.index) && (this->m_bs == rhs.m_bs);
        }

        bool operator!=(const const_iterator &rhs) const { return !(this->operator==(rhs)); }

        friend const_iterator bitset_iterator::begin() const;
        friend const_iterator bitset_iterator::end() const;

    protected:
        std::size_t index;

    private:
        void seek_next()
        {
            const auto bts = (m_bs.to_ulong() >> (++(this->index)));
            switch (bts - (bts & bts - 1))
            {
                case 0:
                    this->index = N;
                case 1 << 0:
                    break;
                case 1 << 1:
                    this->index += 1;
                    break;
                case 1 << 2:
                    this->index += 2;
                    break;
                case 1 << 3:
                    this->index += 3;
                    break;
                case 1 << 4:
                    this->index += 4;
                    break;
                case 1 << 5:
                    this->index += 5;
                    break;
                case 1 << 6:
                    this->index += 6;
                    break;
                case 1 << 7:
                    this->index += 7;
                    break;
                case 1 << 8:
                    this->index += 8;
                    break;
                default:
                    throw std::out_of_range("bitset_iterator doesn't support N > 9");
            }
        }

        const std::bitset<N> &m_bs;
    };

    explicit bitset_iterator(const std::bitset<N> &bitset) : m_bitset(bitset) {}

    std::size_t front() const { return *this->begin(); }

    const_iterator begin() const
    {
        const_iterator iterator{this->m_bitset};
        iterator.seek_next();
        return iterator;
    }

    const_iterator end() const
    {
        const_iterator iterator{this->m_bitset};
        iterator.index = N;
        return iterator;
    }

private:
    BS m_bitset;
};

template <std::size_t N> bitset_iterator<N, const std::bitset<N> &> bitset_it(const std::bitset<N> &bitset)
{
    return bitset_iterator<N, const std::bitset<N> &>(bitset);
}

template <std::size_t N> bitset_iterator<N, const std::bitset<N>> bitset_it(std::bitset<N> &&bitset)
{
    return bitset_iterator<N, const std::bitset<N>>(bitset);
}

} // namespace sudoku::utils

#endif // SUDOKU_UTILS_H
