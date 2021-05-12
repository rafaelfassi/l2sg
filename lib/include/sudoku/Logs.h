#ifndef SUDOKU_LOGS_H
#define SUDOKU_LOGS_H

#include "Enums.h"
#include <vector>
#include <ostream>

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
    Log(Technique _technique) : technique(_technique) {}
    Log() = default;
    void dump(std::ostream &out);
    Technique technique;
    CellLogs cellLogs;
};

using Logs = std::vector<Log>;

} // namespace solver

} // namespace sudoku

#endif // SUDOKU_LOGS_H
