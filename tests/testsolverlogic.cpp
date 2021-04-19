#include "testsolverlogic.h"
#include "grid.h"
#include "solverbruteforce.h"
#include "solverlogic.h"
#include <iostream>
#include <sstream>

void printMsg(const std::string &testName, const std::string &msg)
{
    std::cout << std::endl;
    std::cout << "***************************************************************" << std::endl;
    std::cout << testName << " " << msg << std::endl;
    std::cout << "***************************************************************" << std::endl;
    std::cout << std::endl;
}

bool test(const std::string &unitName, int level, const std::string &puzzle,
          const std::string &expectedResult)
{
    printMsg(unitName, "Startint test...");

    Grid grid;
    grid.setValues(puzzle);
    grid.fillNotes();

    SolverLogic solverLogic(grid);
    solverLogic.solve();

    if (!grid.isFull())
    {
        printMsg(unitName, "Not solved");
        return false;
    }

    int expectedResultArr[9 * 9];
    if (!grid.fillArrayFormString(expectedResult, expectedResultArr))
    {
        printMsg(unitName, "The provided expectedResult is snot valid");
        return false;
    }

    if (expectedResultArr)
    {
        if (!grid.compareValues(expectedResultArr))
        {
            printMsg(unitName, "Solution error");
            return false;
        }
    }

    if (solverLogic.getResultLevel() != level)
    {
        std::stringstream ss;
        ss << "Level Error. Expected: " << std::to_string(level)
           << " Outcome: " << std::to_string(solverLogic.getResultLevel());
        printMsg(unitName, ss.str());
        return false;
    }

    printMsg(unitName, "Passed");
    return true;
}

bool testEasy()
{
    if (!test("0_0000", 0,
              "910208030003100085000034100072003496090000002354006708630000000045070001081005000",
              "917258634463197285528634179172583496896741352354926718639812547245379861781465923"))
        return false;

    if (!test("0_0001", 0,
              "740000000800000000000090010000800030000405000002000690009030000000000005000700008",
              "743681259891257463625394817417869532936425781582173694279538146168942375354716928"))
        return false;

    if (!test("0_0002", 0,
              "700300000000000823085142000590604008000050704004718609003000002900500470000090006",
              "729386145641975823385142967597634218816259734234718659473861592968523471152497386"))
        return false;

    if (!test("0_0003", 0,
              "700060009020008000030000040090000300008002600006050080070000020000200030900070004",
              "785364219624918573139527846297486351548132697316759482873641925461295738952873164"))
        return false;

    return true;
}

bool testMedium()
{
    if (!test("1_0000", 1,
              "600720009000040030300000067000000083000807000570000000950000001020050000400031002",
              "645723819817649235392185467164592783239867154578314926953278641721456398486931572"))
        return false;

    if (!test("1_0001", 1,
              "700600300068000009000510400002000090000843000030000700004095000900000520005001006",
              "741629385568374219329518467452167893197843652836952741674295138913786524285431976"))
        return false;

    return true;
}

bool testHard()
{
    if (!test("2_0000", 2,
              "030961080160008029984700100020300000610875042040009070091087600470100098050090000",
              "732961485165438729984752163827346951619875342543219876291587634476123598358694217"))
        return false;

    if (!test("2_0001", 2,
              "380600000002007000590000000074060003003010900100090620000000095000800200000002074",
              "387629451412587369596143782974268513623715948158394627231476895749851236865932174"))
        return false;

    return true;
}

bool testGuesses()
{
    if (!test("3_0000", 3,
              "12.48.7....6......78...1......8.56...3..9.1.......4.....2.63..83.....2.7.472.93..",
              "123486759456972831789531462294815673538697124671324985912763548365148297847259316"))
        return false;

    if (!test("3_0001", 3,
              "1....7..5.56.9.21....5..6....1.......9.2...5...8.7...43......78....6..3..758.3..6",
              "123647895456398217789521643541936782697284351238175964362459178814762539975813426"))
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