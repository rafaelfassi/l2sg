#include "l2sg/Generator.h"
#include "l2sg/Solver.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace l2sg;

void showHelp(const std::string &appName)
{
    std::cout << "Usage: " << appName << " <options>" << std::endl
              << "Options:" << std::endl
              << "\t-h, --help\t\tShow this help message." << std::endl
              << "\t-f, --file <file>\tLoads a puzzle from the provided file and solve it. See file 'puzzle_fmt.txt'"
              << std::endl
              << "\t-p, --puzzle <puzzle>\tSolve the provided puzzle." << std::endl
              << "\t-g, --generate <level>\tGenerates a new puzzle. The level must be a number from 1 to 5" << std::endl
              << std::endl
              << "\tExamples:" << std::endl
              << "\t\tSolve a provided puzzle:" << std::endl
              << "\t\t\t" << appName
              << " -p 2346.............4.......5.....2.4.8..5....6274...9...86.9.5.3.3...1...6........9" << std::endl
              << "\t\tGenerate a medium level puzzle:" << std::endl
              << "\t\t\t" << appName << " -g 2" << std::endl;
}

int solvePuzzle(const std::string &puzzle)
{
    Grid grid;
    if (!grid.fillAny(puzzle))
    {
        std::cerr << "The provided puzzle is not in a valid format: " << puzzle << std::endl;
        return -1;
    }

    std::cout << std::endl;

    if (grid.isNotesEmpty())
    {
        grid.fillNotes(); // Generates the initial notes (clues) to the solver.
    }
    else if (!grid.hasFullNotes())
    {
        std::cerr << "The provided pencil marks are not full. Please fill all candidates or do not provide any." << std::endl;
        return -1;
    }

    solver::Logs logs;
    Level resultLevel = solver::solve(grid, &logs);

    // Print the solver logs
    for (auto &log : logs)
    {
        log.dump(std::cout);
    }

    std::cout << std::endl;
    std::cout << std::string(9 * 4, '*') << std::endl << std::endl;

    if (resultLevel == Level::LEV_0_NONE)
    {
        std::cout << "The provided puzzle is completed already." << std::endl;
        std::cout << std::endl;
        grid.dump(std::cout, Grid::D_VALUES);
    }
    else if (resultLevel == Level::LEV_INVALID)
    {
        std::cout << "The provided puzzle is not valid." << std::endl;
        std::cout << std::endl;
        grid.dump(std::cout, Grid::D_VALUES | Grid::D_NOTES);
    }
    else if (resultLevel == Level::LEV_UNKNOWN)
    {
        std::cout << "Unable to solve the puzzle with the maximum level provided." << std::endl;
        std::cout << std::endl;
        grid.dump(std::cout, Grid::D_VALUES | Grid::D_NOTES);
    }
    else if (resultLevel == Level::LEV_6_MULTI)
    {
        std::cout << "The provided puzzle has more than one solution." << std::endl;
        std::cout << "Showing the first solution." << std::endl;
        std::cout << std::endl;
        grid.dump(std::cout, Grid::D_VALUES);
    }
    else
    {
        std::cout << "Puzzle level: " << (int)resultLevel << " - " << level2Str(resultLevel) << std::endl;
        std::cout << std::endl;
        grid.dump(std::cout, Grid::D_VALUES);
    }
    std::cout << std::endl;

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        showHelp(argv[0]);
        return 0;
    }

    std::string opt = argv[1];
    if ((opt == "-h") || (opt == "--help"))
    {
        showHelp(argv[0]);
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

        std::string puzzle;
        puzzle.assign((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();
        return solvePuzzle(puzzle);
    }
    else if ((opt == "-p") || (opt == "--puzzle"))
    {
        if (argc < 3)
        {
            std::cerr << "Missing puzzle" << std::endl;
            return -1;
        }
        return solvePuzzle(argv[2]);
    }
    else if ((opt == "-g") || (opt == "--generate"))
    {
        if (argc < 3)
        {
            std::cerr << "Missing level" << std::endl;
            return -1;
        }

        int level = ::std::stoi(argv[2]);
        if ((level < Level::LEV_1_LOGIC) || (level > Level::LEV_5_BRUTE))
        {
            std::cerr << "Invalid level. Please provide a level from 1 to 5" << std::endl;
            return -1;
        }

        Grid grid;
        generator::generateByLevel(grid, static_cast<Level>(level));
        grid.dump(std::cout, Grid::D_VALUES);
        std::cout << std::endl;

        std::cout << "In one line:" << std::endl;
        grid.dump(std::cout, Grid::D_VALUES | Grid::D_ONE_LINE, ".", "", "", "");
        std::cout << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "The provided option is not valid: " << opt << std::endl << "Use -h to see the help." << std::endl;
        return -1;
    }

    return 0;
}
