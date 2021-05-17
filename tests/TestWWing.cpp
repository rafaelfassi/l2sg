#include "Test.h"

// bool test(const std::string &puzzle, const std::string &board, solver::Technique expectedTechnique,
//           const CellLogs &expectedCellLogs)
// {
//     Grid grid;
//     grid.fillBoard(board);

//     solver::Logs logs;
//     bool changed = solver::techniques::wWing(grid, &logs);

//     return checkAll(puzzle, grid, board, changed, expectedTechnique, expectedCellLogs, logs);
// }

bool test(const std::string &puzzle, const std::string &board, const std::string &expectedBoard)
{
    Grid grid;
    grid.fillBoard(board);

    solver::Logs logs;
    bool changed = solver::techniques::wWing(grid, &logs);

    // generateCellLogsCodeForTesting(puzzle, logs);

    return checkAll(puzzle, grid, board, changed, expectedBoard);
}

int main(int, char **)
{
    // row
    // RM(0,2)  BV(0,3)  .......
    // SL(2,2)  .......  SL(2,5)
    // BV(5,2)  .......  .......
    if (!test("......4.81642835978..4..13.4963..8...81.6934....8.46.9572.489.3613975284948.327..",
              R"(
                .---------------.--------------.--------------.
                | 37   235  579 | 57  159  167 | 4  26    8   |
                | 1    6    4   | 2   8    3   | 5  9     7   |
                | 8    25   579 | 4   59   67  | 1  3     26  |
                :---------------+--------------+--------------:
                | 4    9    6   | 3   125  17  | 8  1257  125 |
                | 27   8    1   | 57  6    9   | 3  4     25  |
                | 237  35   57  | 8   12   4   | 6  127   9   |
                :---------------+--------------+--------------:
                | 5    7    2   | 16  4    8   | 9  16    3   |
                | 6    1    3   | 9   7    5   | 2  8     4   |
                | 9    4    8   | 16  3    2   | 7  156   156 |
                '---------------'--------------'--------------'
              )",
              R"(
                .---------------.--------------.--------------.
                | 37   235  79  | 57  159  167 | 4  26    8   |
                | 1    6    4   | 2   8    3   | 5  9     7   |
                | 8    25   579 | 4   59   67  | 1  3     26  |
                :---------------+--------------+--------------:
                | 4    9    6   | 3   125  17  | 8  1257  125 |
                | 27   8    1   | 57  6    9   | 3  4     25  |
                | 237  35   57  | 8   12   4   | 6  127   9   |
                :---------------+--------------+--------------:
                | 5    7    2   | 16  4    8   | 9  16    3   |
                | 6    1    3   | 9   7    5   | 2  8     4   |
                | 9    4    8   | 16  3    2   | 7  156   156 |
                '---------------'--------------'--------------'
              )"))
        return failed();

    // row
    // SL(1,1)  SL(1,8)
    // BV(3,1)  RM(3,8)
    // RM(4,1)  BV(4,8)
    if (!test("91.486.3.5.693241..347159.6..31.4.7...15.73..725893164359278641168349752...651893",
              R"(
                .-------------.----------.-------------.
                | 9    1   27 | 4  8   6 | 25  3   57  |
                | 5    78  6  | 9  3   2 | 4   1   78  |
                | 28   3   4  | 7  1   5 | 9   28  6   |
                :-------------+----------+-------------:
                | 68   89  3  | 1  26  4 | 25  7   589 |
                | 468  49  1  | 5  26  7 | 3   28  89  |
                | 7    2   5  | 8  9   3 | 1   6   4   |
                :-------------+----------+-------------:
                | 3    5   9  | 2  7   8 | 6   4   1   |
                | 1    6   8  | 3  4   9 | 7   5   2   |
                | 24   47  27 | 6  5   1 | 8   9   3   |
                '-------------'----------'-------------'
              )",
              R"(
                .-------------.----------.------------.
                | 9    1   27 | 4  8   6 | 25  3   57 |
                | 5    78  6  | 9  3   2 | 4   1   78 |
                | 28   3   4  | 7  1   5 | 9   28  6  |
                :-------------+----------+------------:
                | 68   89  3  | 1  26  4 | 25  7   58 |
                | 468  4   1  | 5  26  7 | 3   28  89 |
                | 7    2   5  | 8  9   3 | 1   6   4  |
                :-------------+----------+------------:
                | 3    5   9  | 2  7   8 | 6   4   1  |
                | 1    6   8  | 3  4   9 | 7   5   2  |
                | 24   47  27 | 6  5   1 | 8   9   3  |
                '-------------'----------'------------'
              )"))
        return failed();

    // row/block
    // BV(1,0)  .......
    // .......  RM(2,1)
    // RM(3,0)  .......
    // RM(4,0)  BV(4,1)
    // SL(7,0)  SL(7,1)
    if (!test("941852376.7.9361.46.....5.9.1.....57....9..3.78.....4.8.7..9215..21..498194285763",
              R"(
                .----------------.-----------------.-------------.
                | 9    4    1    | 8     5    2    | 3    7   6  |
                | 25   7    58   | 9     3    6    | 1    28  4  |
                | 6    23   38   | 47    147  147  | 5    28  9  |
                :----------------+-----------------+-------------:
                | 234  1    369  | 46    246  348  | 689  5   7  |
                | 245  25   56   | 4567  9    1478 | 68   3   12 |
                | 7    8    3569 | 356   126  13   | 69   4   12 |
                :----------------+-----------------+-------------:
                | 8    36   7    | 346   46   9    | 2    1   5  |
                | 35   356  2    | 1     67   37   | 4    9   8  |
                | 1    9    4    | 2     8    5    | 7    6   3  |
                '----------------'-----------------'-------------'
              )",
              R"(
                .---------------.-----------------.-------------.
                | 9   4    1    | 8     5    2    | 3    7   6  |
                | 25  7    58   | 9     3    6    | 1    28  4  |
                | 6   3    38   | 47    147  147  | 5    28  9  |
                :---------------+-----------------+-------------:
                | 34  1    369  | 46    246  348  | 689  5   7  |
                | 45  25   56   | 4567  9    1478 | 68   3   12 |
                | 7   8    3569 | 356   126  13   | 69   4   12 |
                :---------------+-----------------+-------------:
                | 8   36   7    | 346   46   9    | 2    1   5  |
                | 35  356  2    | 1     67   37   | 4    9   8  |
                | 1   9    4    | 2     8    5    | 7    6   3  |
                '---------------'-----------------'-------------'
              )"))
        return failed();

    // column
    // .......  BV(0,4)  .......
    // RM(2,3)  .......  SL(1,5)
    // BV(8,3)  RM(8,4)  SL(8,5)
    if (!test("593...4..48156.32.267.345.184.3......12.4.8.3.3...8..43786..942956423..8124...635",
              R"(
                .-----------.----------------.----------------.
                | 5   9  3  | 12  78    12   | 4    678   67  |
                | 4   8  1  | 5   6     79   | 3    2     79  |
                | 2   6  7  | 89  3     4    | 5    89    1   |
                :-----------+----------------+----------------:
                | 8   4  59 | 3   1579  1256 | 127  1567  679 |
                | 67  1  2  | 79  4     56   | 8    5679  3   |
                | 67  3  59 | 12  1579  8    | 127  1567  4   |
                :-----------+----------------+----------------:
                | 3   7  8  | 6   15    15   | 9    4     2   |
                | 9   5  6  | 4   2     3    | 17   17    8   |
                | 1   2  4  | 78  789   79   | 6    3     5   |
                '-----------'----------------'----------------'
              )",
              R"(
                .-----------.----------------.----------------.
                | 5   9  3  | 12  78    12   | 4    678   67  |
                | 4   8  1  | 5   6     79   | 3    2     79  |
                | 2   6  7  | 9   3     4    | 5    89    1   |
                :-----------+----------------+----------------:
                | 8   4  59 | 3   1579  1256 | 127  1567  679 |
                | 67  1  2  | 79  4     56   | 8    5679  3   |
                | 67  3  59 | 12  1579  8    | 127  1567  4   |
                :-----------+----------------+----------------:
                | 3   7  8  | 6   15    15   | 9    4     2   |
                | 9   5  6  | 4   2     3    | 17   17    8   |
                | 1   2  4  | 78  79    79   | 6    3     5   |
                '-----------'----------------'----------------'
              )"))
        return failed();

    // column
    // SL(0,0)  BV(0,2)  RM(0,8)
    // SL(8,0)  .......  BV(8,8)
    if (!test("...74238.7..1.5..2.2..3617.942317568687254913315...72429.57..3.8.....2.7.76.23...",
              R"(
                .--------------.--------------.---------------.
                | 15   56  19  | 7    4    2  | 3    8    569 |
                | 7    36  389 | 1    89   5  | 46   49   2   |
                | 45   2   489 | 89   3    6  | 1    7    59  |
                :--------------+--------------+---------------:
                | 9    4   2   | 3    1    7  | 5    6    8   |
                | 6    8   7   | 2    5    4  | 9    1    3   |
                | 3    1   5   | 689  689  89 | 7    2    4   |
                :--------------+--------------+---------------:
                | 2    9   14  | 5    7    18 | 468  3    16  |
                | 8    35  134 | 469  69   19 | 2    459  7   |
                | 145  7   6   | 489  2    3  | 48   459  19  |
                '--------------'--------------'---------------'
              )",
              R"(
                .--------------.--------------.--------------.
                | 15   56  19  | 7    4    2  | 3    8    56 |
                | 7    36  389 | 1    89   5  | 46   49   2  |
                | 45   2   489 | 89   3    6  | 1    7    59 |
                :--------------+--------------+--------------:
                | 9    4   2   | 3    1    7  | 5    6    8  |
                | 6    8   7   | 2    5    4  | 9    1    3  |
                | 3    1   5   | 689  689  89 | 7    2    4  |
                :--------------+--------------+--------------:
                | 2    9   14  | 5    7    18 | 468  3    16 |
                | 8    35  134 | 469  69   19 | 2    459  7  |
                | 145  7   6   | 489  2    3  | 48   459  19 |
                '--------------'--------------'--------------'
              )"))
        return failed();

    // column/block
    // .......  BV(0,2) SL(0,3) ....... RM(0,8)
    // RM(2,0)  ....... SL(2,3) BV(2,6) .......
    if (!test("47..6925...634597..95.27.4696.5847.25487.26.9..79.65846.247..9.7.429386..3965.427",
              R"(
                .--------------.------------.-------------.
                | 4    7    13 | 18  6   9  | 2   5   138 |
                | 128  128  6  | 3   4   5  | 9   7   18  |
                | 138  9    5  | 18  2   7  | 13  4   6   |
                :--------------+------------+-------------:
                | 9    6    13 | 5   8   4  | 7   13  2   |
                | 5    4    8  | 7   13  2  | 6   13  9   |
                | 123  12   7  | 9   13  6  | 5   8   4   |
                :--------------+------------+-------------:
                | 6    158  2  | 4   7   18 | 13  9   135 |
                | 7    15   4  | 2   9   3  | 8   6   15  |
                | 18   3    9  | 6   5   18 | 4   2   7   |
                '--------------'------------'-------------'
              )",
              R"(
                .--------------.------------.-------------.
                | 4    7    13 | 18  6   9  | 2   5   18  |
                | 128  128  6  | 3   4   5  | 9   7   18  |
                | 18   9    5  | 18  2   7  | 13  4   6   |
                :--------------+------------+-------------:
                | 9    6    13 | 5   8   4  | 7   13  2   |
                | 5    4    8  | 7   13  2  | 6   13  9   |
                | 123  12   7  | 9   13  6  | 5   8   4   |
                :--------------+------------+-------------:
                | 6    158  2  | 4   7   18 | 13  9   135 |
                | 7    15   4  | 2   9   3  | 8   6   15  |
                | 18   3    9  | 6   5   18 | 4   2   7   |
                '--------------'------------'-------------'
              )"))
        return failed();

    // block
    // RM(0,0)  .......  SL(0,3)  .......  BV(0,8)
    // .......  BV(2,2)  .......  SL(2,5)  RM(2,8)
    if (!test("..7.8934...9..2..1...7..92.462913...953...612178256439.31..4...7.......4.9462.1.3",
              R"(
                .---------------.---------------.-------------------.
                | 256   12   7  | 15   8    9   | 3     4      56   |
                | 3568  48   9  | 34   346  2   | 578   5678   1    |
                | 3568  148  56 | 7    346  15  | 9     2      568  |
                :---------------+---------------+-------------------:
                | 4     6    2  | 9    1    3   | 578   578    578  |
                | 9     5    3  | 48   47   78  | 6     1      2    |
                | 1     7    8  | 2    5    6   | 4     3      9    |
                :---------------+---------------+-------------------:
                | 2568  3    1  | 58   79   4   | 2578  56789  5678 |
                | 7     28   56 | 138  39   158 | 258   5689   4    |
                | 58    9    4  | 6    2    578 | 1     578    3    |
                '---------------'---------------'-------------------'
              )",
              R"(
                .---------------.---------------.-------------------.
                | 25    12   7  | 15   8    9   | 3     4      56   |
                | 3568  48   9  | 34   346  2   | 578   5678   1    |
                | 3568  148  56 | 7    346  15  | 9     2      58   |
                :---------------+---------------+-------------------:
                | 4     6    2  | 9    1    3   | 578   578    578  |
                | 9     5    3  | 48   47   78  | 6     1      2    |
                | 1     7    8  | 2    5    6   | 4     3      9    |
                :---------------+---------------+-------------------:
                | 2568  3    1  | 58   79   4   | 2578  56789  5678 |
                | 7     28   56 | 138  39   158 | 258   5689   4    |
                | 58    9    4  | 6    2    578 | 1     578    3    |
                '---------------'---------------'-------------------'
              )"))
        return failed();

    // block
    // BV(3,0)  .......  .......
    // SL(6,0)  .......  .......
    // RM(7,0)  SL(7,1)  BV(7,5)
    if (!test("....54..742.7.1..557...31...4.1365783..897..1817425396.34618.52...54..83...37..1.",
              R"(
                .--------------.------------.------------.
                | 16    89  13 | 29  5   4  | 28  36  7  |
                | 4     2   39 | 7   68  1  | 89  36  5  |
                | 5     7   68 | 29  68  3  | 1   24  49 |
                :--------------+------------+------------:
                | 29    4   29 | 1   3   6  | 5   7   8  |
                | 3     56  56 | 8   9   7  | 24  24  1  |
                | 8     1   7  | 4   2   5  | 3   9   6  |
                :--------------+------------+------------:
                | 79    3   4  | 6   1   8  | 79  5   2  |
                | 1267  69  12 | 5   4   29 | 67  8   3  |
                | 26    58  58 | 3   7   29 | 46  1   49 |
                '--------------'------------'------------'
              )",
              R"(
                .-------------.------------.------------.
                | 16   89  13 | 29  5   4  | 28  36  7  |
                | 4    2   39 | 7   68  1  | 89  36  5  |
                | 5    7   68 | 29  68  3  | 1   24  49 |
                :-------------+------------+------------:
                | 29   4   29 | 1   3   6  | 5   7   8  |
                | 3    56  56 | 8   9   7  | 24  24  1  |
                | 8    1   7  | 4   2   5  | 3   9   6  |
                :-------------+------------+------------:
                | 79   3   4  | 6   1   8  | 79  5   2  |
                | 167  69  12 | 5   4   29 | 67  8   3  |
                | 26   58  58 | 3   7   29 | 46  1   49 |
                '-------------'------------'------------'
              )"))
        return failed();


//     if (!test(".23.69......314.........63.57...61..9.6.7.2.8..4....76.81.........298....9.65148.",
//               R"(
// .--------------.------------.------------.
// | 16    89  13 | 29  5   4  | 28  36  7  |
// | 4     2   39 | 7   68  1  | 89  36  5  |
// | 5     7   68 | 29  68  3  | 1   24  49 |
// :--------------+------------+------------:
// | 29    4   29 | 1   3   6  | 5   7   8  |
// | 3     56  56 | 8   9   7  | 24  24  1  |
// | 8     1   7  | 4   2   5  | 3   9   6  |
// :--------------+------------+------------:
// | 79    3   4  | 6   1   8  | 79  5   2  |
// | 1267  69  12 | 5   4   29 | 67  8   3  |
// | 26    58  58 | 3   7   29 | 46  1   49 |
// '--------------'------------'------------'
//               )",
//               R"(
// .-------------.------------.------------.
// | 16   89  13 | 29  5   4  | 28  36  7  |
// | 4    2   39 | 7   68  1  | 89  36  5  |
// | 5    7   68 | 29  68  3  | 1   24  49 |
// :-------------+------------+------------:
// | 29   4   29 | 1   3   6  | 5   7   8  |
// | 3    56  56 | 8   9   7  | 24  24  1  |
// | 8    1   7  | 4   2   5  | 3   9   6  |
// :-------------+------------+------------:
// | 79   3   4  | 6   1   8  | 79  5   2  |
// | 167  69  12 | 5   4   29 | 67  8   3  |
// | 26   58  58 | 3   7   29 | 46  1   49 |
// '-------------'------------'------------'
//               )"))
//         return failed();

    return passed();
}