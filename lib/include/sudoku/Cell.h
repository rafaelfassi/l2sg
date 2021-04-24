#ifndef CELL_H
#define CELL_H

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
    inline void setNotes(const Notes &_nNotes) { m_nNotes = _nNotes; }
    inline bool hasNote(int _nNoteVal) const { return m_nNotes.test(_nNoteVal - 1); };
    inline void setNote(int _nNoteVal, bool _bActive) { m_nNotes.set(_nNoteVal - 1, _bActive); }
    inline size_t notesCount() const { return m_nNotes.count(); };
    inline bool hasAnyNote() const { return m_nNotes.any(); }
    inline void clearNotes() { m_nNotes.reset(); }
    inline void getNotesLst(std::vector<int> &list) const { getNotesLst(m_nNotes, list); }
    static inline void getNotesLst(const Notes &_nNotes, std::vector<int> &list)
    {
        list.reserve(9);

        if (_nNotes.test(0))
            list.push_back(1);

        if (_nNotes.test(1))
            list.push_back(2);

        if (_nNotes.test(2))
            list.push_back(3);

        if (_nNotes.test(3))
            list.push_back(4);

        if (_nNotes.test(4))
            list.push_back(5);

        if (_nNotes.test(5))
            list.push_back(6);

        if (_nNotes.test(6))
            list.push_back(7);

        if (_nNotes.test(7))
            list.push_back(8);

        if (_nNotes.test(8))
            list.push_back(9);
    }

private:
    int m_nValue;
    Notes m_nNotes;
};

} // namespace sudoku

#endif // CELL_H
