#include "sudoku/Solver.h"
#include <chrono>
#include <fstream>
#include <iostream>

// clang-format off

int grade[9*9] =  {0,0,6,  0,0,3,  0,0,0,
                   5,4,0,  8,0,0,  2,0,9,
                   0,0,0,  0,0,9,  6,1,0,

                   0,0,0,  0,8,1,  0,0,0,
                   4,0,0,  0,9,0,  1,6,8,
                   0,8,0,  0,3,4,  5,0,0,

                   0,0,4,  3,7,0,  0,0,0,
                   2,6,7,  9,4,8,  3,5,1,
                   3,0,0,  0,0,0,  4,0,0};

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
        auto resultLevel = Solver::solveLevel(grid);

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
            solved = (Solver::solveByGuesses(grid) == 1);
        }
        else
        {
            resultLevel = Solver::solveLevel(grid);
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
        solved = (Solver::solveByGuesses(grid) == 1);
    }
    else
    {
        resultLevel = Solver::solveLevel(grid, maxLevel);
        solved = (resultLevel != Level::LEV_UNKNOWN);
    }

    grid.dump();
    grid.dump(Grid::D_VALUES | Grid::D_ONE_LINE, ".", "", "", "");
    grid.dump(Grid::D_NOTES, ".", "|", "", "|  ");
    std::cout << "Level: " << resultLevel << std::endl;
    std::cout << "Solved: " << solved << std::endl;
}

void test()
{

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

    switch (SOLVE_ONE)
    {
        case SOLVE_ONE:
            solveOne("8.......5.16...79..9.4.1.3...25.96......3......18.79...4.7.8.1..68...37.9.......8",
                    R"(
                    .........|.23...7..|.........|  .23..6..9|.2...67.9|.23..6...|  .........|.2...6...|.........|
                    .23.5....|.........|.........|  .23......|.........|.23.5....|  .........|.........|.........|
                    .2..5.7..|.........|....5.7..|  .........|.2..567..|.........|  .........|.........|.2...6...|

                    ..34..7..|..3...78.|.........|  .........|1..4.....|.........|  .........|...4...8.|1.....7..|
                    ...4.67..|......78.|.........|  12...6...|.........|.2.4.6...|  .2.45....|.2.45..8.|1.....7..|
                    ...4.6...|.........|.........|  .........|.2.4.6...|.........|  .........|.2.4.....|.........|

                    .2..5....|.........|.........|  .........|.....6..9|.........|  .2..5....|.........|.....6..9|
                    .........|.........|.........|  .2......9|...45....|...45....|  .........|.........|.2......9|
                    .........|.2....7..|....5.7..|  123..6...|12...6...|.23..6...|  .2.45....|.2.456...|.........|
                    )"
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
