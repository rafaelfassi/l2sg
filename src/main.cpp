#include "grid.h"
#include "solverbruteforce.h"
#include "solverlogic.h"
#include <chrono>
#include <fstream>
#include <iostream>

// clang-format off
//int grade[9*9] =  {0,0,0,  0,0,0,  0,0,0,
//                   0,0,0,  0,0,0,  0,0,0,
//                   0,0,0,  0,0,0,  0,0,0,

//                   0,0,0,  0,0,0,  0,0,0,
//                   0,0,0,  0,0,0,  0,0,0,
//                   0,0,0,  0,0,0,  0,0,0,

//                   0,0,0,  0,0,0,  0,0,0,
//                   0,0,0,  0,0,0,  0,0,0,
//                   0,0,0,  0,0,0,  0,0,0};


//Original
//int grade[9*9] =  {0,5,0,  0,1,0,  0,6,0,
//                   9,0,0,  0,0,2,  0,0,4,
//                   0,0,0,  5,0,7,  0,0,0,

//                   0,7,2,  0,0,0,  8,0,0,
//                   1,0,0,  0,0,0,  0,0,5,
//                   0,0,8,  0,0,0,  3,7,0,

//                   0,8,0,  3,0,6,  0,0,0,
//                   6,0,0,  4,0,0,  0,0,1,
//                   0,4,0,  0,2,0,  0,5,0};


// Oririnal
//int grade[9*9] =  {0,5,0,  0,0,0,  0,7,9,
//                   0,0,7,  0,3,0,  0,0,0,
//                   0,4,0,  0,0,1,  0,0,2,

//                   3,0,0,  0,0,0,  0,9,0,
//                   0,2,0,  0,0,6,  0,3,0,
//                   0,1,0,  3,2,0,  0,0,8,

//                   8,0,0,  7,0,0,  0,6,0,
//                   0,0,0,  0,5,0,  9,0,0,
//                   0,6,0,  0,0,0,  0,5,0};

// Travado
//int grade[9*9] =  {0,5,0,  0,6,4,  3,7,9,
//                   0,0,7,  0,3,0,  0,1,0,
//                   0,4,0,  0,7,1,  0,8,2,

//                   3,0,0,  0,0,0,  2,9,0,
//                   0,2,0,  0,0,6,  0,3,0,
//                   0,1,0,  3,2,0,  0,4,8,

//                   8,0,5,  7,0,2,  0,6,0,
//                   0,0,0,  6,5,8,  9,2,0,
//                   0,6,0,  0,0,3,  8,5,0};


// Extremo
//int grade[9*9] =  {2,0,0,  0,7,0,  0,0,8,
//                   0,0,6,  0,0,0,  2,0,0,
//                   0,5,0,  1,0,2,  0,9,0,

//                   0,0,5,  0,0,0,  1,0,0,
//                   3,0,0,  0,2,0,  0,0,4,
//                   0,0,4,  0,0,0,  3,0,0,

//                   0,6,0,  2,0,7,  0,5,0,
//                   0,0,1,  0,0,0,  9,0,0,
//                   4,0,0,  0,3,0,  0,0,6};


//int grade[9*9] =  {2,0,0,  0,7,0,  5,0,8,
//                   0,0,6,  0,5,0,  2,0,0,
//                   0,5,0,  1,4,2,  6,9,0,

//                   0,0,5,  0,0,0,  1,0,9,
//                   3,0,0,  0,2,0,  0,0,4,
//                   0,0,4,  0,0,0,  3,0,5,

//                   0,6,0,  2,0,7,  4,5,0,
//                   5,0,1,  0,0,0,  9,0,2,
//                   4,0,2,  0,3,0,  0,0,6};

//int grade[9*9] =  {0,8,0,  7,0,0,  0,1,0,
//                   3,0,0,  4,0,0,  0,0,0,
//                   4,0,0,  0,0,0,  0,0,5,

//                   0,0,9,  0,0,0,  0,8,0,
//                   6,0,0,  0,9,0,  0,0,2,
//                   0,2,0,  0,1,0,  7,0,0,

//                   8,0,0,  0,0,0,  0,0,9,
//                   0,0,0,  0,0,2,  0,0,8,
//                   0,3,0,  0,0,1,  0,4,0};


//int grade[9*9] =  {0,0,3,  8,0,0,  4,0,0,
//                   7,0,0,  0,0,0,  0,0,0,
//                   5,0,0,  0,0,6,  1,0,0,

//                   0,0,5,  0,2,0,  0,0,3,
//                   0,6,0,  0,0,0,  0,5,0,
//                   1,0,0,  4,0,0,  2,0,0,

//                   0,0,7,  5,0,0,  0,0,8,
//                   0,0,0,  0,0,0,  0,0,6,
//                   0,0,8,  0,0,4,  5,0,0};


//int grade[9*9] =  {0,0,6,  0,0,0,  2,1,7,
//                   0,3,0,  0,2,0,  0,0,0,
//                   7,0,0,  6,0,0,  0,0,0,

//                   0,1,3,  5,8,0,  4,2,0,
//                   0,0,2,  3,0,0,  0,0,5,
//                   8,5,0,  0,0,4,  6,0,0,

//                   0,8,0,  0,0,0,  0,6,2,
//                   0,0,7,  0,0,0,  3,0,0,
//                   0,0,0,  0,0,0,  0,4,0};

int grade[9*9] =  {0,0,6,  0,0,3,  0,0,0,
                   5,4,0,  8,0,0,  2,0,9,
                   0,0,0,  0,0,9,  6,1,0,

                   0,0,0,  0,8,1,  0,0,0,
                   4,0,0,  0,9,0,  1,6,8,
                   0,8,0,  0,3,4,  5,0,0,

                   0,0,4,  3,7,0,  0,0,0,
                   2,6,7,  9,4,8,  3,5,1,
                   3,0,0,  0,0,0,  4,0,0};



/*

  i=0; j=0 => l=0; c=0
  i=0; j=1 => l=0; c=1
  i=0; j=2 => l=0; c=2
  i=0; j=3 => l=1; c=0
  i=0; j=4 => l=1; c=1
  i=0; j=5 => l=1; c=2
  i=0; j=6 => l=2; c=0
  i=0; j=7 => l=2; c=1
  i=0; j=8 => l=2; c=2

  i=1; j=0 => l=0; c=3
  i=1; j=1 => l=0; c=4
  i=1; j=2 => l=0; c=5
  i=1; j=3 => l=1; c=3
  i=1; j=4 => l=1; c=4
  i=1; j=5 => l=1; c=5
  i=1; j=6 => l=2; c=3
  i=1; j=7 => l=2; c=4
  i=1; j=8 => l=2; c=5

  i=2; j=0 => l=0; c=6
  i=2; j=1 => l=0; c=7
  i=2; j=2 => l=0; c=8
  i=2; j=3 => l=1; c=6
  i=2; j=4 => l=1; c=7
  i=2; j=5 => l=1; c=8
  i=2; j=6 => l=2; c=6
  i=2; j=7 => l=2; c=7
  i=2; j=8 => l=2; c=8

  i=3; j=0 => l=3; c=0
  i=3; j=1 => l=3; c=1
  i=3; j=2 => l=3; c=2
  i=3; j=3 => l=4; c=0
  i=3; j=4 => l=4; c=1
  i=3; j=5 => l=4; c=2
  i=3; j=6 => l=5; c=0
  i=3; j=7 => l=5; c=1
  i=3; j=8 => l=5; c=2

*/

// clang-format on

int main()
{
    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();

    //  Grid grid;
    //  // grid.setValues(grade);
    //  grid.setValues("12.48.7....6......78...1......8.56...3..9.1.......4.....2.63..83.....2.7.472.93..");
    //  SolverBruteForce solv(grid);
    //  std::vector<Grid> solutions = solv.solveSolutions(2);
    //  for(size_t x = 0; x < solutions.size(); x++)
    //  {
    //      std::cout << "Solution " << x+1 << std::endl;
    //      solutions[x].dump();
    //  }

    Grid grid;
    // grid.setValues(grade);
    grid.setValues("12.48.7....6......78...1......8.56...3..9.1.......4.....2.63..83.....2.7.472.93..");
    grid.fillNotes();
    // grid.dump();

    SolverLogic solverLogic(grid);
    solverLogic.solve();
    grid.dump();
    grid.dump(Grid::D_ONE_LINE, "_", "", "", "");
    std::cout << "Level:" << solverLogic.getResultLevel() << std::endl;

    // std::ifstream infile("/home/rafael/Dev/SudokuSolver/puzzels/tdoku/data/puzzles1_unbiased");
    // std::string line;
    // size_t maxPuzzles = 1000;//std::numeric_limits<int>::max();
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

    //  TestSolverLogic test;
    //  test.execTest();

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count()
              << "[ms]" << std::endl;
    return 0;
}
