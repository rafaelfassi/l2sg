#include "sudoku/Solver.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace sudoku;

int main(int argc, char *argv[])
{
    std::string puzzle;
    if (argc < 2)
    {
        puzzle = "8.......5.16...79..9.4.1.3...25.96......3......18.79...4.7.8.1..68...37.9.......8";
        std::cout << "No puzzle provided!" << std::endl
                  << "Solving the following puzzle example:" << std::endl
                  << puzzle << std::endl;
    }
    else
    {
        std::string opt = argv[1];
        if ((opt == "-h") || (opt == "--help"))
        {
            std::cout << "Usage: " << argv[0] << " <option(s)>" << std::endl
                      << "Options:" << std::endl
                      << "\t-h,--help\t\tShow this help message." << std::endl
                      << "\t-f,--file <file>\tLoad a puzzle from the provided file and solve it. See the "
                         "file 'puzzle.txt'"
                      << std::endl
                      << "\t<puzzle>\t\tSolve the provided puzzle." << std::endl
                      << std::endl
                      << "\tExample of puzzle: "
                         "8.......5.16...79..9.4.1.3...25.96......3......18.79...4.7.8.1..68...37.9.......8"
                      << std::endl;
            return 0;
        }
        else if ((opt == "-f") || (opt == "--file"))
        {
            if (argc < 3)
            {
                std::cerr << "Missing the file path" << std::endl;
                return -1;
            }

            std::string filePath(argv[2]);
            if (!std::filesystem::is_regular_file(filePath))
            {
                std::cerr << "The provided file is not valid: " << filePath << std::endl;
                return -1;
            }
            std::ifstream inFile(filePath, std::ifstream::in);
            if (inFile.fail())
            {
                std::cerr << "Failed to open the provided file: " << filePath << std::endl;
                return -1;
            }
            puzzle.assign((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            inFile.close();
        }
        else if (opt.size() >= 9 * 9)
        {
            puzzle = opt;
        }
        else
        {
            std::cerr << "The provided option is not valid: " << opt << std::endl
                      << "Use -h to see the help." << std::endl;
            return -1;
        }

        Grid grid;
        if (!grid.fillValues(puzzle))
        {
            std::cerr << "The provided puzzle is not valid: " << puzzle << std::endl;
            return -1;
        }

        std::cout << std::endl;

        grid.fillNotes(); // Generates the initial notes (clues) to the solver.
        Level resultLevel = solver::solveLevel(grid);
        if (resultLevel == Level::LEV_UNKNOWN)
        {
            std::cout << "The provided puzzle could not be solved." << std::endl;
            std::cout << std::endl;
            grid.dump(Grid::D_VALUES | Grid::D_NOTES);
        }
        else
        {
            switch (resultLevel)
            {
                case Level::LEV_0_LOGIC:
                    std::cout << "The puzzle is level: 0 - Easy" << std::endl;
                    break;
                case Level::LEV_1_LOGIC:
                    std::cout << "The puzzle is level: 1 - Medium" << std::endl;
                    break;
                case Level::LEV_2_LOGIC:
                    std::cout << "The puzzle is level: 2 - Hard" << std::endl;
                    break;
                case Level::LEV_3_GUESS:
                    std::cout << "The puzzle is level: 3 - Very Hard, and maybe it can't be solved by logic"
                              << std::endl;
                    break;
                default:
                    break;
            }
            std::cout << std::endl;
            grid.dump(Grid::D_VALUES);
        }
    }

    return 0;
}
