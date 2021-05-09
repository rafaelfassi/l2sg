#ifndef SUDOKU_TEST_H
#define SUDOKU_TEST_H

#include "sudoku/Solver.h"
#include <iostream>

using namespace sudoku;

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

bool checkGrid(const std::string &puzzle, Grid &gridIni, Grid &gridSolved, Grid &gridExpected, bool values,
               bool notes)
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

void checkChanged(const std::string &puzzle, bool changed)
{
    if (!changed)
        printMsg(puzzle, "Not changed returned");
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

bool checkAll(const std::string &puzzle, Grid &grid, const std::string &iniBoard,
              const std::string &expectedBoard, bool changed)
{
    Grid gridIni;
    gridIni.fillBoard(iniBoard);

    Grid gridExpected;
    gridExpected.fillBoard(expectedBoard);

    checkChanged(puzzle, changed);

    return checkGrid(puzzle, gridIni, grid, gridExpected, true, true) && changed;
}

#endif // SUDOKU_TEST_H
