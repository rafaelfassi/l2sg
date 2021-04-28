#include "sudoku/Solver.h"
#include <iostream>
#include <sstream>

using namespace sudoku;

bool test(int maxMultiples, const std::string &puzzle, const std::string &notes,
          const std::string &expectedNotes)
{
    Grid grid;
    grid.fillValues(puzzle);
    grid.fillNotes(notes);

    Solver::reduceCandidatesByHiddenMultiples(grid, maxMultiples);

    Grid gridExpectedNotes;
    gridExpectedNotes.fillNotes(expectedNotes);
    if (!grid.compareNotes(gridExpectedNotes))
    {
        std::cout << std::string(9 * 9, '*') << std::endl;
        std::cout << puzzle << std::endl;
        std::cout << "The notes don't match the expected notes." << std::endl;
        std::cout << std::endl;
        std::cout << "Initial:" << std::endl;
        Grid gridIn;
        gridIn.fillNotes(notes);
        gridIn.dump(Grid::D_NOTES);
        std::cout << "Actual:" << std::endl;
        grid.dump(Grid::D_NOTES);
        std::cout << "Expected:" << std::endl;
        gridExpectedNotes.dump(Grid::D_NOTES);
        std::cout << std::string(9 * 9, '*') << std::endl;
        return false;
    }

    return true;
}

int main(int, char **)
{
    // Hidden pair column for values 1, 9 at:
    // (4,8)
    // (6,8)
    // if (!test(2, ".49132....81479...327685914.96.518...75.28....38.46..5853267...712894563964513...",
    //           R"(
    //             ....56...|.........|.........|  .........|.........|.........|  .....67..|....5.78.|.....678.|
    //             ....56...|.........|.........|  .........|.........|.........|  .23..6...|.23.5....|.2...6...|
    //             .........|.........|.........|  .........|.........|.........|  .........|.........|.........|

    //             .2.4.....|.........|.........|  ..3...7..|.........|.........|  .........|.234..7..|.2....7..|
    //             1..4.....|.........|.........|  ..3.....9|.........|.........|  1.34.6...|..34....9|1....6..9|
    //             12.......|.........|.........|  ......7.9|.........|.........|  12....7..|.2....7.9|.........|

    //             .........|.........|.........|  .........|.........|.........|  1..4.....|...4....9|1.......9|
    //             .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
    //             .........|.........|.........|  .........|.........|.........|  .2....7..|.2....78.|.2....78.|
    //           )",
    //           R"(
    //             ....56...|.........|.........|  .........|.........|.........|  .....67..|....5.78.|.....678.|
    //             ....56...|.........|.........|  .........|.........|.........|  .23..6...|.23.5....|.2...6...|
    //             .........|.........|.........|  .........|.........|.........|  .........|.........|.........|

    //             .2.4.....|.........|.........|  ..3...7..|.........|.........|  .........|.234..7..|.2....7..|
    //             1..4.....|.........|.........|  ..3.....9|.........|.........|  1.34.6...|..34....9|1.......9|
    //             12.......|.........|.........|  ......7.9|.........|.........|  12....7..|.2....7.9|.........|

    //             .........|.........|.........|  .........|.........|.........|  1..4.....|...4....9|1.......9|
    //             .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
    //             .........|.........|.........|  .........|.........|.........|  .2....7..|.2....78.|.2....78.|
    //           )"))
    //     return 1;

    // Hidden pair row for values 4, 7 at:
    // (0,0) (0,1)
    if (!test(2, "....6........42736..673..4..94....68....964.76.7.5.9231......85.6..8.271..5.1..94",
              R"(
                .2345.789|12345.78.|123....89|  1...5..89|.........|1...5..89|  1...5..8.|1...5....|.2......9|
                ....5..89|1...5..8.|1......89|  1...5..89|.........|.........|  .........|.........|.........|
                .2..5..89|12..5..8.|.........|  .........|.........|1...5..89|  1...5..8.|.........|.2......9|

                .23.5....|.........|.........|  123......|.2....7..|1.3...7..|  1...5....|.........|.........|
                .23.5..8.|123.5..8.|123....8.|  123....8.|.........|.........|  .........|1...5....|.........|
                .........|1......8.|.........|  1..4...8.|.........|1..4...8.|  .........|.........|.........|

                .........|.234..7..|.23.....9|  .234.6..9|.2....7..|..34..7.9|  ..3..6...|.........|.........|
                ..34....9|.........|..3.....9|  ..345...9|.........|..345...9|  .........|.........|.........|
                .23...78.|.23...78.|.........|  .23..6...|.........|..3...7..|  ..3..6...|.........|.........|
              )",
              R"(
                ...4..7..|...4..7..|123....89|  1...5..89|.........|1...5..89|  1...5..8.|1...5....|.2......9|
                ....5..89|1...5..8.|1......89|  1...5..89|.........|.........|  .........|.........|.........|
                .2..5..89|12..5..8.|.........|  .........|.........|1...5..89|  1...5..8.|.........|.2......9|

                .23.5....|.........|.........|  123......|.2....7..|1.3...7..|  1...5....|.........|.........|
                .23.5..8.|123.5..8.|123....8.|  123....8.|.........|.........|  .........|1...5....|.........|
                .........|1......8.|.........|  1..4...8.|.........|1..4...8.|  .........|.........|.........|

                .........|.234..7..|.23.....9|  .234.6..9|.2....7..|..34..7.9|  ..3..6...|.........|.........|
                ..34....9|.........|..3.....9|  ..345...9|.........|..345...9|  .........|.........|.........|
                .23...78.|.23...78.|.........|  .23..6...|.........|..3...7..|  ..3..6...|.........|.........|
              )"))
        return 1;

    std::cout << "All tests passed!!!" << std::endl;

    return 0;
}