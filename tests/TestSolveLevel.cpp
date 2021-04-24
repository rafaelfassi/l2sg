#include "sudoku/Solver.h"
#include <iostream>
#include <sstream>

using namespace sudoku;

void printMsg(const std::string &puzzle, const std::string &msg)
{
    std::cout << std::endl;
    std::cout << std::string(9 * 9, '*') << std::endl;
    std::cout << puzzle << std::endl << msg << std::endl;
    std::cout << std::string(9 * 9, '*') << std::endl;
    std::cout << std::endl;
}

bool test(int expectedLevel, const std::string &puzzle, const std::string &expectedResult)
{
    Grid grid;
    grid.setValues(puzzle);
    grid.fillNotes();

    auto resultLevel = Solver::solveLevel(grid);

    if (!grid.isFull())
    {
        printMsg(puzzle, "Not solved");
        return false;
    }

    int expectedResultArr[9 * 9];
    if (!grid.fillArrayFormString(expectedResult, expectedResultArr))
    {
        printMsg(puzzle, "The provided expectedResult is snot valid");
        return false;
    }

    if (!grid.compareValues(expectedResultArr))
    {
        printMsg(puzzle, "Solution error");
        return false;
    }

    if (resultLevel != expectedLevel)
    {
        std::stringstream ss;
        ss << "Level Error. Expected: " << std::to_string(expectedLevel)
           << " Result: " << std::to_string(resultLevel);
        printMsg(puzzle, ss.str());
        return false;
    }

    return true;
}

bool testEasy()
{
    constexpr auto level(Level::LEV_0_LOGIC);

    if (!test(level, //
              "91.2.8.3...31...85....341...72..3496.9......2354..67.863........45.7...1.81..5...",
              "917258634463197285528634179172583496896741352354926718639812547245379861781465923"))
        return false;

    if (!test(level, //
              "74.......8............9..1....8...3....4.5.....2...69...9.3............5...7....8",
              "743681259891257463625394817417869532936425781582173694279538146168942375354716928"))
        return false;

    if (!test(level, //
              "7..3...........823.85142...59.6.4..8....5.7.4..47186.9..3.....29..5..47.....9...6",
              "729386145641975823385142967597634218816259734234718659473861592968523471152497386"))
        return false;

    if (!test(level, //
              "7...6...9.2...8....3.....4..9....3....8..26....6.5..8..7.....2....2...3.9...7...4",
              "785364219624918573139527846297486351548132697316759482873641925461295738952873164"))
        return false;

    return true;
}

bool testMedium()
{
    constexpr auto level(Level::LEV_1_LOGIC);

    if (!test(level, //
              "6..72...9....4..3.3......67.......83...8.7...57.......95......1.2..5....4...31..2",
              "645723819817649235392185467164592783239867154578314926953278641721456398486931572"))
        return false;

    if (!test(level, //
              "7..6..3...68.....9...51.4....2....9....843....3....7....4.95...9.....52...5..1..6",
              "741629385568374219329518467452167893197843652836952741674295138913786524285431976"))
        return false;

    if (!test(level, //
              "38.6.......2..7...59........74.6...3..3.1.9..1...9.62........95...8..2.......2.74",
              "387629451412587369596143782974268513623715948158394627231476895749851236865932174"))
        return false;

    return true;
}

bool testHard()
{
    constexpr auto level(Level::LEV_2_LOGIC);

    if (!test(level, //
              ".3.961.8.16...8.299847..1...2.3.....61.875.42.4...9.7..91.876..47.1...98.5..9....",
              "732961485165438729984752163827346951619875342543219876291587634476123598358694217"))
        return false;

    return true;
}

bool testGuesses()
{
    constexpr auto level(Level::LEV_3_GUESS);

    if (!test(level, //
              "12.48.7....6......78...1......8.56...3..9.1.......4.....2.63..83.....2.7.472.93..",
              "123486759456972831789531462294815673538697124671324985912763548365148297847259316"))
        return false;

    if (!test(level, //
              "1....7..5.56.9.21....5..6....1.......9.2...5...8.7...43......78....6..3..758.3..6",
              "123647895456398217789521643541936782697284351238175964362459178814762539975813426"))
        return false;

    if (!test(level, //
              "98.7.....7.....6....6.5.....4...5.3...79..5......2...1..85..9......1...4.....3.2.",
              "981736245735492618426158397142685739367941582859327461218564973673219854594873126"))
        return false;

    if (!test(level, //
              "98.7..6..75..6......6......8....4....4.....3...9.8.5..2....3..1.9.1...2...8.9.7..",
              "984721653753469182126835479831954267542617938679382514267543891495178326318296745"))
        return false;

    if (!test(level, //
              "98.7..6....7.5..4......9...3.........9..8.2....8..5.97.1..4.9.....5...84....9....",
              "984713652167258349523469871351972468796184235248635197815347926639521784472896513"))
        return false;

    return true;
}

int main(int, char **)
{
    if (!testEasy())
        return 1;

    if (!testMedium())
        return 1;

    if (!testHard())
        return 1;

    if (!testGuesses())
        return 1;

    std::cout << "All tests passed!!!" << std::endl;

    return 0;
}