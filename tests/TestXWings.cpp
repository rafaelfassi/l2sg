#include "Test.h"

bool test(const std::string &puzzle, const std::string &notes, const std::string &expectedNotes)
{
    Grid grid;
    grid.fillValues(puzzle);
    grid.fillNotes(notes);

    solver::techniques::xWings(grid);

    return checkNotes(grid, puzzle, notes, expectedNotes);
}

int main(int, char **)
{
    // X-Wings row for value 6 at:
    // (2,4) (2,8)
    // (6,4) (6,8)
    if (!test("8.......5.16...79..9.4.1.3...25.96......3......18.79...4.7.8.1..68...37.9.......8",
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
              )",
              R"(
                .........|.23...7..|.........|  .23..6..9|.2....7.9|.23..6...|  .........|.2...6...|.........|
                .23.5....|.........|.........|  .23......|.........|.23.5....|  .........|.........|.........|
                .2..5.7..|.........|....5.7..|  .........|.2..567..|.........|  .........|.........|.2...6...|

                ..34..7..|..3...78.|.........|  .........|1..4.....|.........|  .........|...4...8.|1.....7..|
                ...4.67..|......78.|.........|  12...6...|.........|.2.4.6...|  .2.45....|.2.45..8.|1.....7..|
                ...4.6...|.........|.........|  .........|.2.4.....|.........|  .........|.2.4.....|.........|

                .2..5....|.........|.........|  .........|.....6..9|.........|  .2..5....|.........|.....6..9|
                .........|.........|.........|  .2......9|...45....|...45....|  .........|.........|.2......9|
                .........|.2....7..|....5.7..|  123..6...|12.......|.23..6...|  .2.45....|.2.456...|.........|
              )"))
        return failed();


    // X-Wings column for value 3 at:
    // (6,4) (6,7)
    // (7,4) (7,7)
    if (!test("5....3......5..29.....7..8...57......46..132...93..8....7...9....4..9..6....67.4.",
              R"(
                .........|12....789|12.....8.|  12.4.6.89|12.4...89|.........|  1..4.67..|1....67..|1..4..7..|
                1.34.678.|1.3..678.|1.3....8.|  .........|1..4...8.|...4...8.|  .........|.........|1.34..7..|
                1234....9|123.....9|123......|  12.4.6..9|.........|.2.4.....|  1..456...|.........|1.345....|

                123....8.|123....8.|.........|  .........|.2.4...89|.2.4.6.8.|  1..4.6...|1....6...|1..4....9|
                ......78.|.........|.........|  .......89|....5..89|.........|  .........|.........|....5.7.9|
                12....7..|12....7..|.........|  .........|.2.45....|.2.456...|  .........|1...567..|1..45.7..|

                123..6.8.|123.56.8.|.........|  12.4...8.|12345..8.|.2.45..8.|  .........|1.3.5....|.2.....8.|
                123....8.|123.5..8.|.........|  12.....8.|123.5..8.|.........|  1...5.7..|1.3.5.7..|.........|
                123....89|123.5..89|123....8.|  12.....8.|.........|.........|  1...5....|.........|.2.....8.|
              )",
              R"(
                .........|12....789|12.....8.|  12.4.6.89|12.4...89|.........|  1..4.67..|1....67..|1..4..7..|
                1.34.678.|1.3..678.|1.3....8.|  .........|1..4...8.|...4...8.|  .........|.........|1.34..7..|
                1234....9|123.....9|123......|  12.4.6..9|.........|.2.4.....|  1..456...|.........|1.345....|

                123....8.|123....8.|.........|  .........|.2.4...89|.2.4.6.8.|  1..4.6...|1....6...|1..4....9|
                ......78.|.........|.........|  .......89|....5..89|.........|  .........|.........|....5.7.9|
                12....7..|12....7..|.........|  .........|.2.45....|.2.456...|  .........|1...567..|1..45.7..|

                12...6.8.|12..56.8.|.........|  12.4...8.|12345..8.|.2.45..8.|  .........|1.3.5....|.2.....8.|
                12.....8.|12..5..8.|.........|  12.....8.|123.5..8.|.........|  1...5.7..|1.3.5.7..|.........|
                123....89|123.5..89|123....8.|  12.....8.|.........|.........|  1...5....|.........|.2.....8.|
              )"))
        return failed();

    return passed();
}