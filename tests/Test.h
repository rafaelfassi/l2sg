#ifndef SUDOKU_TEST_H
#define SUDOKU_TEST_H

#include "sudoku/Solver.h"
#include <iostream>
#include <set>
#include <sstream>

using namespace sudoku;
using CL = CellLog;
using CA = CellAction;

int passed()
{
    std::cout << "All tests passed!!!" << std::endl;
    return 0;
}

int failed()
{
    return 1;
}

void printMsg(const std::string &puzzle, const std::string &msg)
{
    std::cout << std::endl;
    std::cout << std::string(9 * 9, '*') << std::endl;
    std::cout << puzzle << std::endl << msg << std::endl;
    std::cout << std::string(9 * 9, '*') << std::endl;
    std::cout << std::endl;
}

bool checkGrid(const std::string &puzzle, Grid &gridIni, Grid &gridSolved, Grid &gridExpected, bool values, bool notes)
{

    bool ok(true);

    if (values)
    {
        if (!gridSolved.compareValues(gridExpected))
        {
            std::cout << std::string(9 * 9, '*') << std::endl;

            std::cout << puzzle << std::endl;
            std::cout << "The values don't match the expected ones." << std::endl;
            std::cout << std::endl;

            std::cout << "Initial ";
            gridIni.dump(Grid::D_VALUES);
            std::cout << std::endl;

            std::cout << "Expected ";
            gridExpected.dump(Grid::D_VALUES);
            std::cout << std::endl;

            std::cout << "Actual ";
            gridSolved.dump(Grid::D_VALUES);
            std::cout << std::endl;

            std::cout << std::string(9 * 9, '*') << std::endl;
            ok &= false;
        }
    }

    if (notes)
    {
        if (!gridSolved.compareNotes(gridExpected))
        {
            std::cout << std::string(9 * 9, '*') << std::endl;

            std::cout << puzzle << std::endl;
            std::cout << "The notes don't match the expected ones." << std::endl;
            std::cout << std::endl;

            std::cout << "Initial ";
            gridIni.dump(Grid::D_NOTES);
            std::cout << std::endl;

            std::cout << "Expected ";
            gridExpected.dump(Grid::D_NOTES);
            std::cout << std::endl;

            std::cout << "Actual ";
            gridSolved.dump(Grid::D_NOTES);
            std::cout << std::endl;

            std::cout << std::string(9 * 9, '*') << std::endl;
            ok &= false;
        }
    }

    return ok;
}

bool checkChanged(const std::string &puzzle, bool changed)
{
    if (!changed)
        printMsg(puzzle, "Not changed returned");
    return changed;
}

bool checkValues(const std::string &puzzle, Grid &grid, const std::string &expectedValues)
{
    Grid gridIni;
    gridIni.fillValues(puzzle);

    Grid gridExpected;
    gridExpected.fillValues(expectedValues);

    return checkGrid(puzzle, gridIni, grid, gridExpected, true, false);
}

bool checkNotes(const std::string &puzzle, Grid &grid, const std::string &initialNotes,
                const std::string &expectedNotes)
{
    Grid gridIni;
    gridIni.fillNotes(initialNotes);

    Grid gridExpected;
    gridExpected.fillNotes(expectedNotes);

    return checkGrid(puzzle, gridIni, grid, gridExpected, false, true);
}

bool checkAll(const std::string &puzzle, Grid &grid, const std::string &iniBoard, const std::string &expectedBoard,
              bool changed)
{
    Grid gridIni;
    gridIni.fillBoard(iniBoard);

    Grid gridExpected;
    gridExpected.fillBoard(expectedBoard);

    checkChanged(puzzle, changed);

    return checkGrid(puzzle, gridIni, grid, gridExpected, true, true) && changed;
}

bool checkAll(const std::string &puzzle, Grid &grid, const std::string &iniBoard, bool changed,
              solver::Technique expectedTechnique, const CellLogs &expectedLogs, solver::Logs &solverLogs)
{
    bool ok(checkChanged(puzzle, changed));

    Grid gridPuzzle;
    gridPuzzle.fillValues(puzzle);

    Grid gridIni;
    gridIni.fillBoard(iniBoard);

    Grid gridExpected(gridIni);

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (gridPuzzle.getValue(i, j) && (gridPuzzle.getValue(i, j) != gridIni.getValue(i, j)))
            {
                printMsg(puzzle, "The provided puzzle and board don't match");
                ok &= false;
            }
        }
    }

    if (solverLogs.empty())
    {
        printMsg(puzzle, "The 'solverLogs' is empty");
        return false;
    }

    if (solverLogs.front().technique != expectedTechnique)
    {
        std::stringstream ss;
        ss << "Technique doesn't match." << std::endl
           << "Expected: " << solver::technique2Str(expectedTechnique) << std::endl
           << "Actual: " << solver::technique2Str(solverLogs.front().technique);
        printMsg(puzzle, ss.str());
        ok &= false;
    }

    CellLogs &logs = solverLogs.front().cellLogs;

    if (logs.size() != expectedLogs.size())
    {
        printMsg(puzzle, "Different log sizes");
        ok &= false;
    }

    const auto dmpCellLog = [](std::ostream &out, const CellLog &cellLog)
    {
        out << "Action: " << cellAction2Str(cellLog.action)                //
            << " At: " << cellLog.cell.first << "," << cellLog.cell.second //
            << " Value: " << cellLog.value;
    };

    for (const auto &exLog : expectedLogs)
    {
        auto it = std::find_if(
            logs.begin(), logs.end(),
            [&exLog](const CellLog &log)
            { return ((log.action == exLog.action) && (log.cell == exLog.cell) && (log.value == exLog.value)); });

        if (it != logs.end())
        {
            if (it->action == CellAction::RemovedNote)
            {
                if (!gridExpected.hasNote(it->cell.first, it->cell.second, it->value))
                {
                    std::stringstream ss;
                    ss << "The note " << it->value << " to be removed from " //
                       << it->cell.first << "," << it->cell.second << " is not set.";
                    printMsg(puzzle, ss.str());
                    return false;
                }
                gridExpected.setNote(it->cell.first, it->cell.second, it->value, false);
            }
            else if (it->action == CellAction::SetValue)
            {
                if (gridExpected.getValue(it->cell.first, it->cell.second) > 0)
                {
                    std::stringstream ss;
                    ss << "The value " << it->value << " to be set at " //
                       << it->cell.first << "," << it->cell.second << " is already set.";
                    printMsg(puzzle, ss.str());
                    return false;
                }
                gridExpected.setValue(it->cell.first, it->cell.second, it->value);
                gridExpected.clearNotesCascade(it->cell.first, it->cell.second, it->value);
            }
            if (it != logs.end() - 1)
                *it = std::move(logs.back());
            logs.pop_back();
        }
        else
        {
            std::stringstream ss;
            ss << "Expected log was not found." << std::endl;
            dmpCellLog(ss, exLog);
            printMsg(puzzle, ss.str());
            return false;
        }
    }

    if (!logs.empty())
    {
        std::stringstream ss;
        ss << logs.size() << " not expected changes found.";
        for (const auto &log : logs)
        {
            ss << std::endl;
            dmpCellLog(ss, log);
        }
        printMsg(puzzle, ss.str());
        ok &= false;
    }

    ok &= checkGrid(puzzle, gridIni, grid, gridExpected, true, true);

    return ok;
}

void generateCellLogsCodeForTesting(const std::string &puzzle, solver::Logs &logs)
{
    const auto processCellAction = [&](const std::string &label, std::set<CA> cellActions)
    {
        std::cout << "    // " << label << std::endl;

        int currentRow(-1);
        int colCount(0);
        for (int i = 0; i < logs.front().cellLogs.size(); ++i)
        {
            const CellLog &cellLog = logs.front().cellLogs.at(i);

            if (cellActions.find(cellLog.action) == cellActions.end())
                continue;

            if ((std::max(currentRow, cellLog.cell.first) != currentRow) || (++colCount == 3))
            {
                colCount = 0;
                if (currentRow != -1)
                    std::cout << ", //" << std::endl;
                currentRow = cellLog.cell.first;
                std::cout << "    ";
            }
            else
            {
                std::cout << ", ";
            }

            std::cout << "CL(" << cellLog.cell.first << ", " << cellLog.cell.second
                      << ", CA::" << cellAction2Str(cellLog.action) << " ," << cellLog.value << ")";
        }
    };

    if (logs.empty())
    {
        throw std::out_of_range("'logs' is empty");
    }

    std::sort(logs.front().cellLogs.begin(), logs.front().cellLogs.end(),
              [](const CellLog &a, const CellLog &b)
              {
                  if (a.cell.first != b.cell.first)
                      return a.cell.first < b.cell.first;
                  else if (a.cell.second != b.cell.second)
                      return a.cell.second < b.cell.second;
                  else
                      return a.value < b.value;
              });

    std::cout << "******************************************************************" << std::endl;
    std::cout << puzzle << std::endl << std::endl;

    std::cout << "solver::Technique::" << solver::technique2Str(logs.front().technique) << "," << std::endl;
    std::cout << "{" << std::endl;

    processCellAction("Pattern", {CA::RelatedNote, CA::SetValue});
    std::cout << ", //" << std::endl << std::endl;
    processCellAction("Remove", {CA::RemovedNote});
    std::cout << "  //" << std::endl;

    std::cout << "}" << std::endl;
    std::cout << std::endl;
}

#endif // SUDOKU_TEST_H
