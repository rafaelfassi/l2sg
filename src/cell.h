#ifndef CELL_H
#define CELL_H

#include <bitset>
#include <vector>

class Cell
{
public:
    using Notes = std::bitset<9>;

    Cell();
    int getValue() const { return m_nValue; }
    void setValue(int _nValue) { m_nValue = _nValue; }
    Notes getNotes() const { return m_nNotes; }
    void setNotes(const Notes &_nNotes) { m_nNotes = _nNotes; }
    bool getNoteVisible(int _nNoteVal) const { return m_nNotes.test(_nNoteVal - 1); };
    void setNoteVisible(int _nNoteVal, bool _bVisible) { m_nNotes.set(_nNoteVal - 1, _bVisible); }
    size_t notesCount() const { return m_nNotes.count(); };
    bool hasNote() const { return m_nNotes.any(); }
    void clearNotes() { m_nNotes.reset(); }
    void getVisibleNotesLst(std::vector<int> &list) const;

private:
    int m_nValue;
    Notes m_nNotes;
};

#endif // CELL_H
