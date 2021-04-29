#include "Test.h"

bool test(int maxHidden, const std::string &puzzle, const std::string &notes,
          const std::string &expectedNotes)
{
    Grid grid;
    grid.fillValues(puzzle);
    grid.fillNotes(notes);

    solver::techniques::hiddenMulti(grid, maxHidden);

    return checkNotes(grid, puzzle, notes, expectedNotes);
}

int main(int, char **)
{
    // Hidden pair column for values 1, 9 at:
    // (4,8)
    // (6,8)
    if (!test(2, ".49132....81479...327685914.96.518...75.28....38.46..5853267...712894563964513...",
              R"(
                ....56...|.........|.........|  .........|.........|.........|  .....67..|....5.78.|.....678.|
                ....56...|.........|.........|  .........|.........|.........|  .23..6...|.23.5....|.2...6...|
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|

                .2.4.....|.........|.........|  ..3...7..|.........|.........|  .........|.234..7..|.2....7..|
                1..4.....|.........|.........|  ..3.....9|.........|.........|  1.34.6...|..34....9|1....6..9|
                12.......|.........|.........|  ......7.9|.........|.........|  12....7..|.2....7.9|.........|

                .........|.........|.........|  .........|.........|.........|  1..4.....|...4....9|1.......9|
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
                .........|.........|.........|  .........|.........|.........|  .2....7..|.2....78.|.2....78.|
              )",
              R"(
                ....56...|.........|.........|  .........|.........|.........|  .....67..|....5.78.|.....678.|
                ....56...|.........|.........|  .........|.........|.........|  .23..6...|.23.5....|.2...6...|
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|

                .2.4.....|.........|.........|  ..3...7..|.........|.........|  .........|.234..7..|.2....7..|
                1..4.....|.........|.........|  ..3.....9|.........|.........|  1.34.6...|..34....9|1.......9|
                12.......|.........|.........|  ......7.9|.........|.........|  12....7..|.2....7.9|.........|

                .........|.........|.........|  .........|.........|.........|  1..4.....|...4....9|1.......9|
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
                .........|.........|.........|  .........|.........|.........|  .2....7..|.2....78.|.2....78.|
              )"))
        return failed();

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
        return failed();

    // Hidden triple block for values 2, 4, 5 at:
    // (7,1) .....
    // (8,1) (8,2)
    if (!test(3, "28....473534827196.71.34.8.3..5...4....34..6.46.79.31..9.2.3654..3..9821....8.937",
              R"(
                .........|.........|.....6..9|  1....6..9|1...56...|1...56...|  .........|.........|.........|
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
                .....6..9|.........|.........|  .....6..9|.........|.........|  .2..5....|.........|.2..5....|

                .........|12.......|.2....789|  .........|1....6...|12...6.8.|  .2....7..|.........|.2.....89|
                1.....789|12..5....|.2..5.789|  .........|.........|12.....8.|  .2..5.7..|.........|.2..5..89|
                .........|.........|.2..5..8.|  .........|.........|.2.....8.|  .........|.........|.2..5..8.|

                1.....78.|.........|......78.|  .........|1.....7..|.........|  .........|.........|.........|
                .....67..|...45....|.........|  ...4.6...|....567..|.........|  .........|.........|.........|
                1....6...|12.45....|.2..56...|  1..4.6...|.........|1...56...|  .........|.........|.........|
              )",
              R"(
                .........|.........|.....6..9|  1....6..9|1...56...|1...56...|  .........|.........|.........|
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
                .....6..9|.........|.........|  .....6..9|.........|.........|  .2..5....|.........|.2..5....|

                .........|12.......|.2....789|  .........|1....6...|12...6.8.|  .2....7..|.........|.2.....89|
                1.....789|12..5....|.2..5.789|  .........|.........|12.....8.|  .2..5.7..|.........|.2..5..89|
                .........|.........|.2..5..8.|  .........|.........|.2.....8.|  .........|.........|.2..5..8.|

                1.....78.|.........|......78.|  .........|1.....7..|.........|  .........|.........|.........|
                .....67..|...45....|.........|  ...4.6...|....567..|.........|  .........|.........|.........|
                1....6...|.2.45....|.2..5....|  1..4.6...|.........|1...56...|  .........|.........|.........|
              )"))
        return failed();

    // Hidden triple column for values 2, 5, 6 at:
    // (3,5)
    // (5,5)
    // (7,5)
    if (!test(3, "5..62..37..489........5....93........2....6.57.......3.....9.........7..68.57...2",
              R"(
                .........|1.......9|1......89|  .........|.........|1..4.....|  1..4...89|.........|.........|
                123......|1....67..|.........|  .........|.........|1.3...7..|  12..5....|12..56...|1....6...|
                123....8.|1....67.9|123..6789|  1.34..7..|.........|1.34..7..|  12.4...89|12.4.6.89|1..4.6.89|

                .........|.........|1...56.8.|  12.4..7..|1..4.6.8.|12.45678.|  12.4...8.|12.4..78.|1..4...8.|
                1..4...8.|.........|1......8.|  1.34..7.9|1.34...8.|1.34..78.|  .........|1..4..789|.........|
                .........|1..456...|1...56.8.|  12.4....9|1..4.6.8.|12.456.8.|  12.4...89|12.4...89|.........|

                1234.....|1..45.7..|123.5.7..|  1234.....|1.34.6.8.|.........|  1.345..8.|1..456.8.|1..4.6.8.|
                1234.....|1..45...9|123.5...9|  1234.....|1.34.6.8.|1234.6.8.|  .........|1..456.89|1..4.6.89|
                .........|.........|1.3.....9|  .........|.........|1.34.....|  1.34....9|1..4....9|.........|
              )",
              R"(
                .........|1.......9|1......89|  .........|.........|1..4.....|  1..4...89|.........|.........|
                123......|1....67..|.........|  .........|.........|1.3...7..|  12..5....|12..56...|1....6...|
                123....8.|1....67.9|123..6789|  1.34..7..|.........|1.34..7..|  12.4...89|12.4.6.89|1..4.6.89|

                .........|.........|1...56.8.|  12.4..7..|1..4.6.8.|.2..56...|  12.4...8.|12.4..78.|1..4...8.|
                1..4...8.|.........|1......8.|  1.34..7.9|1.34...8.|1.34..78.|  .........|1..4..789|.........|
                .........|1..456...|1...56.8.|  12.4....9|1..4.6.8.|.2..56...|  12.4...89|12.4...89|.........|

                1234.....|1..45.7..|123.5.7..|  1234.....|1.34.6.8.|.........|  1.345..8.|1..456.8.|1..4.6.8.|
                1234.....|1..45...9|123.5...9|  1234.....|1.34.6.8.|.2...6...|  .........|1..456.89|1..4.6.89|
                .........|.........|1.3.....9|  .........|.........|1.34.....|  1.34....9|1..4....9|.........|
              )"))
        return failed();

    // Hidden pair column for values 4, 6 at:
    // (6,6)
    // (7,6)
    // Hidden quadruple block for values 2, 4, 5, 8 at:
    // (6,4) (6,5)
    // (7,4) (7,5)
    if (!test(4, "816573294392......4572.9..6941...5687854961236238...4.279.....1138....7.564....82",
              R"(
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
                .........|.........|.........|  1....6...|1..4.6.8.|1..4...8.|  ......78.|1...5....|....5.7..|
                .........|.........|.........|  .........|1......8.|.........|  ..3....8.|1.3......|.........|

                .........|.........|.........|  ..3...7..|.23......|.2....7..|  .........|.........|.........|
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
                .........|.........|.........|  .........|1...5....|1...5....|  ......7.9|.........|......7.9|

                .........|.........|.........|  ..3..6...|..3456.8.|...45..8.|  ..34.6...|..3.5....|.........|
                .........|.........|.........|  .....6..9|.2.456...|.2.45....|  ...4.6..9|.........|....5...9|
                .........|.........|.........|  1.3...7.9|1.3......|1.....7..|  ..3.....9|.........|.........|
              )",
              R"(
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
                .........|.........|.........|  1....6...|1..4.6.8.|1..4...8.|  ......78.|1...5....|....5.7..|
                .........|.........|.........|  .........|1......8.|.........|  ..3....8.|1.3......|.........|

                .........|.........|.........|  ..3...7..|.23......|.2....7..|  .........|.........|.........|
                .........|.........|.........|  .........|.........|.........|  .........|.........|.........|
                .........|.........|.........|  .........|1...5....|1...5....|  ......7.9|.........|......7.9|

                .........|.........|.........|  ..3..6...|...45..8.|...45..8.|  ...4.6...|..3.5....|.........|
                .........|.........|.........|  .....6..9|.2.45....|.2.45....|  ...4.6...|.........|....5...9|
                .........|.........|.........|  1.3...7.9|1.3......|1.....7..|  ..3.....9|.........|.........|
              )"))
        return failed();

    // Hidden quadruple column for values 5,6,7,9 at:
    // (0,0)
    // (1,0)
    // (4,0)
    // (5,0)
    // Hidden quadruple column for values 2,4,5,9 at:
    // (0,8)
    // (1,8)
    // (2,8)
    // (7,8)
    if (!test(4, ".3.....1...8.9....4..6.8......57694....98352....124...276..519....7.9....95...47.",
              R"(
                ....567.9|.........|.2....7.9|  .2.4.....|...45....|.2....7..|  .2...678.|.........|.2.456789|
                1...567..|12..56...|.........|  .234.....|.........|12....7..|  .23..67..|..3.56...|.234567..|
                .........|12..5....|12....7.9|  .........|1.3.5....|.........|  .23...7..|..3.5....|.23.5.7.9|

                1.3....8.|12.....8.|123......|  .........|.........|.........|  .........|.........|1.3....8.|
                1....67..|1..4.6...|1..4..7..|  .........|.........|.........|  .........|.........|1....67..|
                ..3.56789|....56.8.|..3...7.9|  .........|.........|.........|  ..3..678.|..3..6.8.|..3..678.|

                .........|.........|.........|  ..34...8.|..34.....|.........|  .........|.........|..3....8.|
                1.3....8.|1..4...8.|1.34.....|  .........|1.3..6...|.........|  .23..6.8.|..3.56.8.|.23.56.8.|
                1.3....8.|.........|.........|  .23....8.|1.3..6...|12.......|  .........|.........|..3..6.8.|
              )",
              R"(
                ....567.9|.........|.2....7.9|  .2.4.....|...45....|.2....7..|  .2...678.|.........|.2.45...9|
                ....567..|12..56...|.........|  .234.....|.........|12....7..|  .23..67..|..3.56...|.2.45....|
                .........|12..5....|12....7.9|  .........|1.3.5....|.........|  .23...7..|..3.5....|.2..5...9|

                1.3....8.|12.....8.|123......|  .........|.........|.........|  .........|.........|1.3....8.|
                .....67..|1..4.6...|1..4..7..|  .........|.........|.........|  .........|.........|1....67..|
                ....567.9|....56.8.|..3...7.9|  .........|.........|.........|  ..3..678.|..3..6.8.|..3..678.|

                .........|.........|.........|  ..34...8.|..34.....|.........|  .........|.........|..3....8.|
                1.3....8.|1..4...8.|1.34.....|  .........|1.3..6...|.........|  .23..6.8.|..3.56.8.|.2..5....|
                1.3....8.|.........|.........|  .23....8.|1.3..6...|12.......|  .........|.........|..3..6.8.|
              )"))
        return failed();

    return passed();
}