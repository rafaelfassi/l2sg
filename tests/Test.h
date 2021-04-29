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

bool checkNotes(Grid &grid, const std::string &puzzle, const std::string &initialNotes,
                const std::string &expectedNotes)
{
    Grid gridExpectedNotes;
    gridExpectedNotes.fillNotes(expectedNotes);
    if (!grid.compareNotes(gridExpectedNotes))
    {
        std::cout << std::string(9 * 9, '*') << std::endl;

        std::cout << puzzle << std::endl;
        std::cout << "The notes don't match the expected ones." << std::endl;
        std::cout << std::endl;

        std::cout << "Initial ";
        Grid gridIniNotes;
        gridIniNotes.fillNotes(initialNotes);
        gridIniNotes.dump(Grid::D_NOTES);
        std::cout << std::endl;

        std::cout << "Expected ";
        gridExpectedNotes.dump(Grid::D_NOTES);
        std::cout << std::endl;

        std::cout << "Actual ";
        grid.dump(Grid::D_NOTES);
        std::cout << std::endl;

        std::cout << std::string(9 * 9, '*') << std::endl;
        return false;
    }

    return true;
}

bool checkValues(Grid &grid, const std::string &puzzle, const std::string &expectedValues)
{
    Grid gridExpectedValues;
    gridExpectedValues.fillValues(expectedValues);
    if (!grid.compareValues(gridExpectedValues))
    {
        std::cout << std::string(9 * 9, '*') << std::endl;

        std::cout << puzzle << std::endl;
        std::cout << "The values don't match the expected ones." << std::endl;
        std::cout << std::endl;

        std::cout << "Initial ";
        Grid gridIniValues;
        gridIniValues.fillValues(puzzle);
        gridIniValues.dump(Grid::D_VALUES);
        std::cout << std::endl;

        std::cout << "Expected ";
        gridExpectedValues.dump(Grid::D_VALUES);
        std::cout << std::endl;

        std::cout << "Actual ";
        grid.dump(Grid::D_VALUES);
        std::cout << std::endl;

        std::cout << std::string(9 * 9, '*') << std::endl;
        return false;
    }

    return true;
}

#endif // SUDOKU_TEST_H
