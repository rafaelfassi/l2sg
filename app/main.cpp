#include "sudoku/Solver.h"
#include <chrono>
#include <fstream>
#include <iostream>

// clang-format off

int grade[9*9] =  {1,0,8,  5,0,0,  2,3,4,
                   5,0,0,  3,0,2,  1,7,8,
                   0,0,0,  8,0,0,  5,6,9,

                   8,0,0,  6,0,5,  7,9,3,
                   0,0,5,  9,0,0,  4,8,1,
                   3,0,0,  0,0,8,  6,5,2,

                   9,8,0,  2,0,6,  3,1,0,
                   0,0,0,  0,0,0,  8,0,0,
                   0,0,0,  7,8,0,  9,0,0};

// clang-format on

using namespace sudoku;

void splitFileIntoLevels(const std::string &inFileName, const std::string &outDirPath,
                         size_t maxPuzzles = std::numeric_limits<size_t>::max())
{
    size_t processedPuzzles = 0;
    int levelsCount[Level::LEV_UNKNOWN + 1] = {};
    std::string line;

    std::ifstream inFile(inFileName);
    std::ofstream outEasyFile(outDirPath + "/easy_unsolved.txt", std::ofstream::out | std::ofstream::app);
    std::ofstream outMediumFile(outDirPath + "/medium_unsolved.txt", std::ofstream::out | std::ofstream::app);
    std::ofstream outHardFile(outDirPath + "/hard_unsolved.txt", std::ofstream::out | std::ofstream::app);
    std::ofstream outGuessFile(outDirPath + "/guess_unsolved.txt", std::ofstream::out | std::ofstream::app);

    while (std::getline(inFile, line) && (processedPuzzles++ < maxPuzzles))
    {
        if (line.empty() || !std::isdigit(line.front()))
        {
            --processedPuzzles;
            continue;
        }

        std::string puzzle = line.substr(0, line.find(","));
        Grid grid;
        grid.fillValues(puzzle);
        grid.fillNotes();
        auto resultLevel = solver::solveLevel(grid);

        ++levelsCount[resultLevel];
        switch (resultLevel)
        {
            case Level::LEV_0_LOGIC:
                outEasyFile << puzzle << std::endl;
                break;
            case Level::LEV_1_LOGIC:
                outMediumFile << puzzle << std::endl;
                break;
            case Level::LEV_2_LOGIC:
                outHardFile << puzzle << std::endl;
                break;
            case Level::LEV_3_GUESS:
                outGuessFile << puzzle << std::endl;
                break;
            default:
                std::cout << "Puzzle NOT solved!!! -> " << puzzle << std::endl;
                grid.dump();
                return;
        }
    }

    outEasyFile.flush();
    outEasyFile.close();

    outMediumFile.flush();
    outMediumFile.close();

    outHardFile.flush();
    outHardFile.close();

    outGuessFile.flush();
    outGuessFile.close();

    std::cout << "Easy: " << levelsCount[Level::LEV_0_LOGIC] << " Medium: " << levelsCount[Level::LEV_1_LOGIC]
              << " Hard: " << levelsCount[Level::LEV_2_LOGIC] << " Guess: " << levelsCount[Level::LEV_3_GUESS]
              << std::endl;
}

void executeFromFile(
    const std::string &fileName, bool onlyGuesses, size_t maxPuzzles = std::numeric_limits<size_t>::max(),
    const std::function<bool(Level, int64_t, size_t)> print = [](Level, int64_t, size_t) { return false; })
{
    size_t processedPuzzles(0);
    std::ifstream inFile(fileName);
    Level resultLevel(Level::LEV_UNKNOWN);
    bool solved(false);
    std::string line;

    while (std::getline(inFile, line) && (processedPuzzles++ < maxPuzzles))
    {
        if (line.empty() || !std::isdigit(line.front()))
        {
            --processedPuzzles;
            continue;
        }
        std::string puzzle = line.substr(0, line.find(","));
        Grid grid;
        grid.fillValues(puzzle);
        grid.fillNotes();

        std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();
        if (onlyGuesses)
        {
            solved = (solver::solveByGuesses(grid) == 1);
        }
        else
        {
            resultLevel = solver::solveLevel(grid);
            solved = (resultLevel != Level::LEV_UNKNOWN);
        }
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();

        const auto elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count();

        if (!solved)
        {
            std::cout << "NOT SOLVED!!!" << std::endl;
            std::cout << "Puzzle: " << puzzle << std::endl;
            std::cout << "Level: " << resultLevel << std::endl;
            grid.dump(Grid::D_ONE_LINE, ".", "", "", "");
            std::cout << std::string(100, '*') << std::endl;
        }
        else if (print(resultLevel, elapsedTime, processedPuzzles))
        {
            std::cout << "Puzzle: " << puzzle << std::endl;
            std::cout << "Level: " << resultLevel << std::endl;
            std::cout << "Solve time: " << elapsedTime << "[ms]" << std::endl;
            std::cout << std::string(100, '*') << std::endl;
        }
    }
}

void solveOne(const std::string &puzzle, const std::string &notes, bool onlyGuesses = false, Level maxLevel = Level::LEV_3_GUESS)
{
    Level resultLevel(Level::LEV_UNKNOWN);
    bool solved(false);
    Grid grid;

    grid.fillValues(puzzle);

    if (notes.empty())
        grid.fillNotes();
    else
        grid.fillNotes(notes);

    if (onlyGuesses)
    {
        solved = (solver::solveByGuesses(grid) == 1);
    }
    else
    {
        resultLevel = solver::solveLevel(grid, maxLevel);
        solved = (resultLevel != Level::LEV_UNKNOWN);
    }

    //grid.dump();
    grid.dump(Grid::D_VALUES);
    grid.dump(Grid::D_VALUES | Grid::D_ONE_LINE, ".", "", "", "");
    grid.dump(Grid::D_NOTES, ".", "|", "", "|  ");
    std::cout << "Level: " << resultLevel << std::endl;
    std::cout << "Solved: " << solved << std::endl;
}

int main()
{
    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();

    enum WhatExec
    {
        SOLVE_ONE,
        EXECUTE_FROM_FILE,
        SPLIT_FILE_INTO_LEVELS
    };

    switch (EXECUTE_FROM_FILE)
    {
        case SOLVE_ONE:
            solveOne(R"(
                    .3. ... .1.
                    ..8 .9. ...
                    4.. 6.8 ...

                    ... 576 94.
                    ... 983 52.
                    ... 124 ...

                    276 ..5 19.
                    ... 7.9 ...
                    .95 ... 47.
                    )",
                    ""
                     , false, Level::LEV_2_LOGIC);
            break;
        case EXECUTE_FROM_FILE:
            executeFromFile("/home/rafael/Dev/SudokuSolver/puzzels/tdoku/data/puzzles3_magictour_top1465",
                            false, 100'000, [] (Level level, int64_t time, size_t n){
                                return level == Level::LEV_2_LOGIC;
                            });
            break;
        case SPLIT_FILE_INTO_LEVELS:
            splitFileIntoLevels("/home/rafael/Dev/SudokuSolver/puzzels/tdoku/data/puzzles1_unbiased",
                                "/home/rafael/Dev/SudokuSolver/puzzels/levels");
            break;
        default:
            break;
    }

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    const auto elapsedTime(
        std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count());
    std::cout << "Elapsed time: " << elapsedTime << "[ms]" << std::endl;
    return 0;
}
