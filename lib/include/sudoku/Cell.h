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

    Cell() : m_value(0){};
    inline int getValue() const { return m_value; }
    inline void setValue(int _value) { m_value = _value; }
    inline Notes getNotes() const { return m_notes; }
    inline bool hasNote(int _note) const { return m_notes.test(_note - 1); };
    inline size_t notesCount() const { return m_notes.count(); };
    inline bool hasAnyNote() const { return m_notes.any(); }
    inline void getNotesList(std::vector<int> &list)
    {
        list.reserve(9);
        for (int nIdx = 0; nIdx < m_notes.size(); ++nIdx)
        {
            if (m_notes.test(nIdx))
                list.push_back(nIdx + 1);
        }
    }

private:
    inline void setNote(int _note, bool _active) { m_notes.set(_note - 1, _active); }
    inline void setNotes(const Notes &_notes) { m_notes = _notes; }
    inline void clearNotes() { m_notes.reset(); }
    friend class Grid;

    int m_value;
    Notes m_notes;
};

} // namespace sudoku

#endif // SUDOKU_CELL_H
