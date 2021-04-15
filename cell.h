#ifndef CELL_H
#define CELL_H

#include <vector>

class Cell
{
public:
    enum{
        NOTE_1  = 0x001,
        NOTE_2  = 0x002,
        NOTE_3  = 0x004,
        NOTE_4  = 0x008,
        NOTE_5  = 0x010,
        NOTE_6  = 0x020,
        NOTE_7  = 0x040,
        NOTE_8  = 0x080,
        NOTE_9  = 0x100
    };

    Cell();
    int getValue() { return m_nValue; }
    void setValue(int _nValue) { m_nValue = _nValue; }
    int getNotes() { return m_nNotes; }
    void setNotes(int _nNotes) { m_nNotes = _nNotes; }
    bool getNoteVisible(int _nNoteVal);
    void setNoteVisible(int _nNoteVal, bool _bVisible);
    size_t notesCount();
    void clearNotes();
    std::vector<int> getVisibleNotesLst();

    static size_t notesCount(int _nNotes);
    static bool getNoteVisible(int _nNoteVal, int _nNotes);
    static std::vector<int> getVisibleNotesLst(int _nNotes);

private:
    int m_nValue;
    int m_nNotes;
};

#endif // CELL_H
