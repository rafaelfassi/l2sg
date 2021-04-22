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
    void getVisibleNotesLst(std::vector<int> &list) const { getVisibleNotesLst(m_nNotes, list); }
    static void getVisibleNotesLst(const Notes &_nNotes, std::vector<int> &list)
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

#endif // CELL_H
