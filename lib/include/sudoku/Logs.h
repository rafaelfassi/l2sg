#ifndef SUDOKU_LOGS_H
#define SUDOKU_LOGS_H

#include "Enums.h"
#include <ostream>
#include <vector>

namespace sudoku
{

struct CellLog
{
    CellLog(const std::pair<int, int> &_cell, CellAction _action, int _value)
        : cell(_cell), action(_action), value(_value)
    {
    }
    CellLog(int row, int col, CellAction _action, int _value)
        : cell(std::make_pair(row, col)), action(_action), value(_value)
    {
    }
    CellLog() = default;
    std::pair<int, int> cell;
    CellAction action;
    int value;
};

using CellLogs = std::vector<CellLog>;

namespace solver
{

struct Log
{
    Log(Technique _technique = Technique::None) : technique(_technique) {}
    void dump(std::ostream &out);
    Technique technique;
    CellLogs cellLogs;
};

using Logs = std::vector<Log>;

class ScopedLog
{
    Log m_log;
    Logs *m_logs;

public:
        ScopedLog(Logs *_logs, Technique _technique = Technique::None) : m_logs(_logs), m_log(_technique) {}
    ~ScopedLog()
    {
        if (m_logs && !m_log.cellLogs.empty())
        {
            m_logs->push_back(std::move(m_log));
        }
    }
    inline bool isEnabled() { return (m_logs != nullptr); }
    inline void addCellLog(int _r, int _c, CellAction _act, int _val)
    {
        if (m_logs)
        {
            m_log.cellLogs.emplace_back(_r, _c, _act, _val);
        }
    }
    inline void addCellLog(const std::pair<int, int> &_cell, CellAction _act, int _val)
    {
        if (m_logs)
        {
            m_log.cellLogs.emplace_back(_cell, _act, _val);
        }
    }
    inline void setTechnique(Technique _technique) { m_log.technique = _technique; }
    inline CellLogs *getCellsPtr() { return (m_logs == nullptr ? nullptr : &m_log.cellLogs); }
};

} // namespace solver

} // namespace sudoku

#endif // SUDOKU_LOGS_H
