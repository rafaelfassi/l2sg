#include "Test.h"
#include <sstream>

bool invalidPuzzle()
{
    Grid grid;
    grid.fillValues(".56.73..2.1.49.....34..48...25..19.4.81.5.72.6.93..15...7...34.....34.1.1..26.58.");
    grid.fillNotes();
    solver::Logs logs;
    if (solver::solve(grid, &logs) != Level::LEV_INVALID)
    {
        printMsg(__func__, "Test failed: Wrong level");
        return false;
    }

    if (logs.empty())
    {
        printMsg(__func__, "Test failed: Logs empty");
        return false;
    }

    if (logs.front().cellLogs.empty())
    {
        printMsg(__func__, "Test failed: CellLogs empty");
        return false;
    }

    const auto cellLog = logs.front().cellLogs.front();
    if (cellLog.action != CellAction::ConflictedValue)
    {
        printMsg(__func__, "Test failed: Wrong action");
        return false;
    }

    if (cellLog.cell != std::make_pair(2, 5))
    {
        printMsg(__func__, "Test failed: Wrong cell");
        return false;
    }

    if (cellLog.value != 4)
    {
        printMsg(__func__, "Test failed: Wrong value");
        return false;
    }

    return true;
}

bool noSolution()
{
    Grid grid;
    grid.fillValues("123.56789...4....................................................................");
    grid.fillNotes();
    solver::Logs logs;
    if (solver::solve(grid, &logs) != Level::LEV_INVALID)
    {
        printMsg(__func__, "Test failed: Wrong level");
        return false;
    }

    if (logs.empty())
    {
        printMsg(__func__, "Test failed: Logs empty");
        return false;
    }

    if (logs.front().cellLogs.empty())
    {
        printMsg(__func__, "Test failed: CellLogs empty");
        return false;
    }

    const auto cellLog = logs.front().cellLogs.front();
    if (cellLog.action != CellAction::MissingNoteRow)
    {
        printMsg(__func__, "Test failed: Wrong action");
        return false;
    }

    if (cellLog.cell != std::make_pair(0, 0))
    {
        printMsg(__func__, "Test failed: Wrong cell");
        return false;
    }

    if (cellLog.value != 4)
    {
        printMsg(__func__, "Test failed: Wrong value");
        return false;
    }

    return true;
}

bool noSolution2()
{
    Grid grid;
    grid.fillValues("...76.......4...718...................7...9..6..3..12..7.....4......6......97....");
    grid.fillNotes();
    if (Level level = solver::solve(grid); level != Level::LEV_INVALID)
    {
        printMsg(__func__, "Test failed due wrong level: " + level2Str(level));
        return false;
    }

    return true;
}

bool multipleSolutions()
{
    Grid grid;
    grid.fillValues("......574.4.15...35......62....64.9.....157.6.6...28.5....49.1.......4....67813..");
    grid.fillNotes();

    if (Level level = solver::solve(grid); level != Level::LEV_6_MULTI)
    {
        printMsg(__func__, "Test failed due wrong level: " + level2Str(level));
        return false;
    }
    return true;
}

bool multipleSolutionsFromNotes()
{
    Grid grid;
    grid.fillBoard(
        R"(
        +-------------------------------+-------------------------------+-------------------------------+
        | 123456789 2345678   3456      | 1234569   1234579   12345689  | 12345689  123456789 12345689  |
        | 23478     2345678   34678     | 123456789 123456789 12345689  | 123456789 234678    2345678   |
        | 2345678   345678    23456789  | 1234569   12345679  12345689  | 123456789 1246789   123456789 |
        +-------------------------------+-------------------------------+-------------------------------+
        | 123456789 123456789 13469     | 12346     1234678   123468    | 123456789 124       12345678  |
        | 13456789  3456789   13456789  | 123456789 123456789 123456789 | 123456789 123456789 123456789 |
        | 13456789  345678    13456789  | 123456789 123456789 123456789 | 123456789 123456789 123456789 |
        +-------------------------------+-------------------------------+-------------------------------+
        | 123456789 23456789  123456789 | 12345679  123456789 12345689  | 123456789 123456789 123456789 |
        | 123456789 2456789   3456789   | 123456789 23456789  123456789 | 123456789 123456789 23456789  |
        | 123456789 23456789  13456789  | 123456789 123456789 12345689  | 123456789 1234678   123456789 |
        +-------------------------------+-------------------------------+-------------------------------+
    )");

    if (solver::solve(grid) != Level::LEV_6_MULTI)
    {
        printMsg(__func__, "Test failed");
        return false;
    }
    return true;
}

bool onlyNotes()
{
    Grid grid;
    grid.fillBoard(
        R"(
        +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
        | 1456789   | 3456789   | 456789    | 12356789  | 2356789   | 13456789  | 123456789 | 123456789 | 123456789 |
        | 123456789 | 3456789   | 456789    | 123456789 | 23456789  | 23456789  | 1245789   | 123456789 | 23456789  |
        | 123456789 | 13456789  | 123456789 | 1256789   | 12789     | 13456789  | 12456789  | 126789    | 12456789  |
        +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
        | 123456789 | 1256789   | 256789    | 123456789 | 26789     | 456789    | 2789      | 1236789   | 23456789  |
        | 123456789 | 23456789  | 23456789  | 3456789   | 234789    | 123456789 | 2349      | 23456789  | 23456789  |
        | 23456789  | 123456789 | 23456789  | 1345789   | 23456789  | 3456789   | 23456789  | 123456789 | 3489      |
        +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
        | 123456789 | 123456789 | 123456789 | 12456789  | 123456789 | 123456789 | 123456789 | 1234569   | 123456789 |
        | 12456789  | 13456789  | 156789    | 156789    | 23456789  | 1456789   | 123456789 | 13456789  | 3456789   |
        | 235679    | 123456789 | 89        | 23456789  | 456789    | 12346789  | 356789    | 123456789 | 456789    |
        +-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
    )");

    if (solver::solve(grid) != Level::LEV_3_LOGIC)
    {
        printMsg(__func__, "Test failed");
        return false;
    }
    return true;
}

bool notesNotProvided()
{
    Grid grid;
    grid.fillValues("973..1.68.45...92.2.6..85.36...83...4.7...6.9...74...23.82..7.5.24...81.79.8..234");

    if (Level level = solver::solve(grid); level != Level::LEV_INVALID)
    {
        printMsg(__func__, "Test failed due wrong level: " + level2Str(level));
        return false;
    }
    return true;
}

bool puzzleFull()
{
    Grid grid;
    grid.fillValues("239485167456371892817926435724539681965817324183642579548293716672158943391764258");

    if (Level level = solver::solve(grid); level != Level::LEV_0_NONE)
    {
        printMsg(__func__, "Test failed due wrong level: " + level2Str(level));
        return false;
    }
    return true;
}

int main(int, char **)
{
    if (!invalidPuzzle())
        return failed();

    if (!noSolution())
        return failed();

    if (!noSolution2())
        return failed();

    if (!multipleSolutions())
        return failed();

    if (!multipleSolutionsFromNotes())
        return failed();

    if (!onlyNotes())
        return failed();

    if (!notesNotProvided())
        return failed();

    if (!puzzleFull())
        return failed();

    return passed();
}