#include "grid.h"
#include "solverbruteforce.h"
#include "solverlogic.h"
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

int main()
{
    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();

    //  Grid grid;
    //  // grid.setValues(grade);
    //  grid.setValues("98.7.....7.....6....6.5.....4...5.3...79..5......2...1..85..9......1...4.....3.2.");
    //  SolverBruteForce solv(grid);
    //  std::vector<Grid> solutions = solv.solveSolutions(2);
    //  for(size_t x = 0; x < solutions.size(); x++)
    //  {
    //      std::cout << "Solution " << x+1 << std::endl;
    //      solutions[x].dump();
    //  }

    Grid grid;
    // grid.setValues(grade);
    grid.setValues("1....7..5.56.9.21....5..6....1.......9.2...5...8.7...43......78....6..3..758.3..6");
    grid.fillNotes();
    // grid.dump();

    SolverLogic solverLogic(grid);
    solverLogic.solve();
    grid.dump();
    grid.dump(Grid::D_ONE_LINE, "_", "", "", "");
    std::cout << "Level:" << solverLogic.getResultLevel() << std::endl;

    // std::ifstream infile("/home/rafael/Dev/SudokuSolver/puzzels/tdoku/data/puzzles4_forum_hardest_1905");
    // std::string line;
    // size_t maxPuzzles = 10; //std::numeric_limits<int>::max();
    // while (maxPuzzles-- && std::getline(infile, line))
    // {
    //     if(line.empty() || !std::isdigit(line.front()))
    //     {
    //       ++maxPuzzles;
    //       continue;
    //     }

    //     std::string puzzle = line.substr(0, line.find(","));
    //     Grid grid;
    //     grid.setValues(puzzle);
    //     grid.fillNotes();
    //     SolverLogic solverLogic(grid);
    //     solverLogic.solve();
    //     if (!grid.isFull())
    //     {
    //       std::cout << "Puzzle: " << puzzle << std::endl;
    //       std::cout << "Level: " << solverLogic.getResultLevel() << std::endl;
    //       std::cout << "Solved: " << grid.isFull() << std::endl;
    //       grid.dump(Grid::D_ONE_LINE, "_", "", "", "");
    //       std::cout << "***********************************************" << std::endl;
    //     }
    // }

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count()
              << "[ms]" << std::endl;
    return 0;
}
