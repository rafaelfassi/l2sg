#include "cell.h"

Cell::Cell() : m_nValue(0), m_nNotes(0)
{
}

bool Cell::getNoteVisible(int _nNoteVal) const
{
    return getNoteVisible(_nNoteVal, m_nNotes);
}

void Cell::setNoteVisible(int _nNoteVal, bool _bVisible)
{
    switch (_nNoteVal)
    {
        case 1:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_1) : (m_nNotes & ~NOTE_1);
            break;
        case 2:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_2) : (m_nNotes & ~NOTE_2);
            break;
        case 3:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_3) : (m_nNotes & ~NOTE_3);
            break;
        case 4:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_4) : (m_nNotes & ~NOTE_4);
            break;
        case 5:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_5) : (m_nNotes & ~NOTE_5);
            break;
        case 6:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_6) : (m_nNotes & ~NOTE_6);
            break;
        case 7:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_7) : (m_nNotes & ~NOTE_7);
            break;
        case 8:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_8) : (m_nNotes & ~NOTE_8);
            break;
        case 9:
            m_nNotes = _bVisible ? (m_nNotes | NOTE_9) : (m_nNotes & ~NOTE_9);
            break;
    }
}

size_t Cell::notesCount() const
{
    return notesCount(m_nNotes);
}

size_t Cell::notesCount(int _nNotes)
{
    int flag = 1, value;
    size_t count(0);

    for (int x = 0; x < 9; ++x)
    {
        value = _nNotes & flag;
        if (value)
            ++count;
        flag = flag << 1;
    }

    return count;
}

void Cell::clearNotes()
{
    m_nNotes = 0;
}

std::vector<int> Cell::getVisibleNotesLst() const
{
    return getVisibleNotesLst(m_nNotes);
}

bool Cell::getNoteVisible(int _nNoteVal, int _nNotes)
{
    switch (_nNoteVal)
    {
        case 1:
            return _nNotes & NOTE_1;
        case 2:
            return _nNotes & NOTE_2;
        case 3:
            return _nNotes & NOTE_3;
        case 4:
            return _nNotes & NOTE_4;
        case 5:
            return _nNotes & NOTE_5;
        case 6:
            return _nNotes & NOTE_6;
        case 7:
            return _nNotes & NOTE_7;
        case 8:
            return _nNotes & NOTE_8;
        case 9:
            return _nNotes & NOTE_9;
        default:
            return false;
    }
}

std::vector<int> Cell::getVisibleNotesLst(int _nNotes)
{
    std::vector<int> lst;
    lst.reserve(9);

    if(_nNotes & NOTE_1)
         lst.push_back(1);

    if(_nNotes & NOTE_2)
         lst.push_back(2);

    if(_nNotes & NOTE_3)
         lst.push_back(3);

    if(_nNotes & NOTE_4)
         lst.push_back(4);

    if(_nNotes & NOTE_5)
         lst.push_back(5);

    if(_nNotes & NOTE_6)
         lst.push_back(6);

    if(_nNotes & NOTE_7)
         lst.push_back(7);

    if(_nNotes & NOTE_8)
         lst.push_back(8);

    if(_nNotes & NOTE_9)
         lst.push_back(9);

    return lst;
}
