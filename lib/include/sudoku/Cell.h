#ifndef SUDOKU_CELL_H
#define SUDOKU_CELL_H

#include <bitset>
#include <vector>

namespace sudoku
{

class Cell
{
public:
    using Notes = std::bitset<9>;

    Cell() : m_nValue(0){};
    inline int getValue() const { return m_nValue; }
    inline void setValue(int _nValue) { m_nValue = _nValue; }
    inline Notes getNotes() const { return m_nNotes; }
    inline bool hasNote(int _nNoteVal) const { return m_nNotes.test(_nNoteVal - 1); };
    inline size_t notesCount() const { return m_nNotes.count(); };
    inline bool hasAnyNote() const { return m_nNotes.any(); }
    inline int getNextNote(int note) const
    {
        if (m_nNotes.to_ulong() >> note)
        {
            for (int i = note; i < m_nNotes.size(); ++i)
            {
                if (m_nNotes.test(i))
                    return i + 1;
            }
        }
        return 0;
    }
    inline void getNotesList(std::vector<int> &list)
    {
        list.reserve(9);
        for (int i = 0; i < m_nNotes.size(); ++i)
        {
            if (m_nNotes.test(i))
                list.push_back(i+1);
        }
    }

private:
    inline void setNote(int _nNoteVal, bool _bActive) { m_nNotes.set(_nNoteVal - 1, _bActive); }
    inline void setNotes(const Notes &_nNotes) { m_nNotes = _nNotes; }
    inline void clearNotes() { m_nNotes.reset(); }
    friend class Grid;

    int m_nValue;
    Notes m_nNotes;
};

} // namespace sudoku

#endif // SUDOKU_CELL_H
