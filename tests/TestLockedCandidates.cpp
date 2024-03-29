#include "Test.h"

bool test(solver::LockedCandType lcType, const std::string &puzzle, const std::string &board,
          solver::Technique expectedTechnique)
{
    Grid grid;
    grid.fillBoard(board);

    solver::Logs logs;
    bool changed = solver::techniques::lockedCandidates(grid, lcType, &logs);

    return checkAll(puzzle, grid, board, changed, expectedTechnique, logs);
}

int main(int, char **)
{
    // Row
    if (!test(solver::LockedCandType::Type1Pointing,
              "2....39.183.6.172..51.2.3..1629458373.....2..785236149.2....593.135.2.785.83...12",
              R"(
                .--------------------.----------------------.-------------.
                | 2    4(*7)  46(*7) | 4(-7)8  5(-7)8  3    | 9   568  1  |
                | 8    3      49     | 6       59      1    | 7   2    45 |
                | 469  5      1      | 478     2       4789 | 3   68   46 |
                :--------------------+----------------------+-------------:
                | 1    6      2      | 9       4       5    | 8   3    7  |
                | 3    49     49     | 178     178     78   | 2   56   56 |
                | 7    8      5      | 2       3       6    | 1   4    9  |
                :--------------------+----------------------+-------------:
                | 46   2      467    | 1478    1678    478  | 5   9    3  |
                | 469  1      3      | 5       69      2    | 46  7    8  |
                | 5    479    8      | 3       679     479  | 46  1    2  |
                '--------------------'----------------------'-------------'
              )",
              solver::Technique::LockedCandidatesType1))
        return failed();

    // Row
    if (!test(solver::LockedCandType::Type1Pointing,
              "4357918..16..5..7372..3..516419273855..16.294.92.456172...8.1499.4.1.73..1.4795..",
              R"(
                .----------------.------------------.-------------.
                | 4   3   5      | 7       9  1     | 8   26  26  |
                | 1   6   89     | 28      5  248   | 49  7   3   |
                | 7   2   89     | 68      3  468   | 49  5   1   |
                :----------------+------------------+-------------:
                | 6   4   1      | 9       2  7     | 3   8   5   |
                | 5   78  37     | 1       6  38    | 2   9   4   |
                | 38  9   2      | 38      4  5     | 6   1   7   |
                :----------------+------------------+-------------:
                | 2   57  (-3)67 | (*3)56  8  (*3)6 | 1   4   9   |
                | 9   58  4      | 256     1  26    | 7   3   68  |
                | 38  1   36     | 4       7  9     | 5   26  268 |
                '----------------'------------------'-------------'
              )",
              solver::Technique::LockedCandidatesType1))
        return failed();

    // Row
    if (!test(solver::LockedCandType::Type1Pointing,
              "...6145974513792....9825134...5.741.91.48..53.451.....59.2413..1.4963.25236758941",
              R"(
                .------------------.-----------.-------------------.
                | 38      28   238 | 6  1   4  | 5       9      7  |
                | 4       5    1   | 3  7   9  | 2       68     68 |
                | 67      67   9   | 8  2   5  | 1       3      4  |
                :------------------+-----------+-------------------:
                | 368     268  238 | 5  39  7  | 4       1      29 |
                | 9       1    27  | 4  8   26 | 67      5      3  |
                | 37(-8)  4    5   | 1  39  26 | 67(*8)  7(*8)  29 |
                :------------------+-----------+-------------------:
                | 5       9    78  | 2  4   1  | 3       678    68 |
                | 1       78   4   | 9  6   3  | 78      2      5  |
                | 2       3    6   | 7  5   8  | 9       4      1  |
                '------------------'-----------'-------------------'
              )",
              solver::Technique::LockedCandidatesType1))
        return failed();

    // Row
    if (!test(solver::LockedCandType::Type1Pointing,
              ".6..9..388.3.1......428316...6851.2..52.7.81..8.32.5...3714568....76.3.161.93..7.",
              R"(
                .----------------------.-------------.---------------------.
                | 1257     6       15  | 45   9  47  | 247   3       8     |
                | 8        279     3   | 456  1  467 | 2479  459     24579 |
                | 579      79      4   | 2    8  3   | 1     6       579   |
                :----------------------+-------------+---------------------:
                | 3479     479     6   | 8    5  1   | 479   2       3479  |
                | 349      5       2   | 46   7  469 | 8     1       3469  |
                | 1479     8       19  | 3    2  469 | 5     49      4679  |
                :----------------------+-------------+---------------------:
                | 29       3       7   | 1    4  5   | 6     8       29    |
                | 2(*4)59  2(*4)9  589 | 7    6  28  | 3     (-4)59  1     |
                | 6        1       58  | 9    3  28  | 24    7       245   |
                '----------------------'-------------'---------------------'
              )",
              solver::Technique::LockedCandidatesType1))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type1Pointing,
              "....3.574...7..891.7981.623761428935..8.7.216.....1748....4315..3...648.2.4.87369",
              R"(
                .-----------------.-------------------.----------.
                | 168   128  26   | (-2)69   3    29  | 5  7  4  |
                | 3456  245  2356 | 7        56   245 | 8  9  1  |
                | 45    7    9    | 8        1    45  | 6  2  3  |
                :-----------------+-------------------+----------:
                | 7     6    1    | 4        2    8   | 9  3  5  |
                | 3459  459  8    | 359      7    59  | 2  1  6  |
                | 359   259  235  | 3569     569  1   | 7  4  8  |
                :-----------------+-------------------+----------:
                | 689   89   67   | (*2)9    4    3   | 1  5  27 |
                | 159   3    57   | 1(*2)59  59   6   | 4  8  27 |
                | 2     15   4    | 15       8    7   | 3  6  9  |
                '-----------------'-------------------'----------'
              )",
              solver::Technique::LockedCandidatesType1))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type1Pointing,
              "61..7.25..4.56179..758..61.13.64.5875.7..8461486157932.647.3125..14.5376753216849",
              R"(
                .-------------.-----------------.-----------.
                | 6    1   89 | 39  7       49  | 2  5  348 |
                | 23   4   28 | 5   6       1   | 7  9  38  |
                | 39   7   5  | 8   23(-9)  249 | 6  1  34  |
                :-------------+-----------------+-----------:
                | 1    3   29 | 6   4       29  | 5  8  7   |
                | 5    29  7  | 39  23(-9)  8   | 4  6  1   |
                | 4    8   6  | 1   5       7   | 9  3  2   |
                :-------------+-----------------+-----------:
                | 89   6   4  | 7   8(*9)   3   | 1  2  5   |
                | 289  29  1  | 4   8(*9)   5   | 3  7  6   |
                | 7    5   3  | 2   1       6   | 8  4  9   |
                '-------------'-----------------'-----------'
              )",
              solver::Technique::LockedCandidatesType1))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type1Pointing,
              "1...6..78...8.2.63.864.75...4.526.......41....1.7.8.24..12.37..73.6.....89.17...6",
              R"(
                .---------------------.--------------.-------------------.
                | 1     25   (-2)3459 | 39  6    59  | 249   7      8    |
                | 459   57   4579     | 8   159  2   | 149   6      3    |
                | 239   8    6        | 4   139  7   | 5     19     129  |
                :---------------------+--------------+-------------------:
                | 39    4    3789     | 5   2    6   | 1389  1389   179  |
                | 2369  267  (-2)3789 | 39  4    1   | 3689  3589   579  |
                | 3569  1    359      | 7   39   8   | 369   2      4    |
                :---------------------+--------------+-------------------:
                | 456   56   1        | 2   589  3   | 7     4589   59   |
                | 7     3    (*2)45   | 6   589  459 | 1289  14589  1259 |
                | 8     9    (*2)45   | 1   7    45  | 23    345    6    |
                '---------------------'--------------'-------------------'
              )",
              solver::Technique::LockedCandidatesType1))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type1Pointing,
              "196.42.35.3...6912.251934..5623.9.41.8346152..1.2..6.36.1.283..3.891.2..259634178",
              R"(
                .-------------.-----------------.------------.
                | 1    9   6  | 78   4       2  | 78  3   5  |
                | 478  3   47 | 578  57(-8)  6  | 9   1   2  |
                | 78   2   5  | 1    9       3  | 4   68  67 |
                :-------------+-----------------+------------:
                | 5    6   2  | 3    7(*8)   9  | 78  4   1  |
                | 79   8   3  | 4    6       1  | 5   2   79 |
                | 479  1   47 | 2    57(*8)  57 | 6   89  3  |
                :-------------+-----------------+------------:
                | 6    47  1  | 57   2       8  | 3   59  49 |
                | 3    47  8  | 9    1       57 | 2   56  46 |
                | 2    5   9  | 6    3       4  | 1   7   8  |
                '-------------'-----------------'------------'
              )",
              solver::Technique::LockedCandidatesType1))
        return failed();

    // Row
    if (!test(solver::LockedCandType::Type2Claiming,
              "9827136453.49..1781..4..932...1397646..8.4.19419.76..374.69...1596.41..7..1..7496",
              R"(
                .-------------.------------.--------------------.
                | 9   8    2  | 7   1   3  | 6        4       5 |
                | 3   56   4  | 9   26  25 | 1        7       8 |
                | 1   567  57 | 4   68  58 | 9        3       2 |
                :-------------+------------+--------------------:
                | 28  25   58 | 1   3   9  | 7        6       4 |
                | 6   37   37 | 8   25  4  | 25       1       9 |
                | 4   1    9  | 25  7   6  | 258      258     3 |
                :-------------+------------+--------------------:
                | 7   4    38 | 6   9   28 | 235(-8)  25(-8)  1 |
                | 5   9    6  | 23  4   1  | 23(*8)   2(*8)   7 |
                | 28  23   1  | 35  58  7  | 4        9       6 |
                '-------------'------------'--------------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    // Row
    if (!test(solver::LockedCandType::Type2Claiming,
              "19.....363741..259.2..3914..1..94.6596..5.41245.62198..319...24.89..357174..1..9.",
              R"(
                .--------------.---------------------------.------------.
                | 1    9  58   | 2457(-8)  47(-8)  257(-8) | 78   3  6  |
                | 3    7  4    | 1         6(*8)   6(*8)   | 2    5  9  |
                | 568  2  568  | 57(-8)    3       9       | 1    4  78 |
                :--------------+---------------------------+------------:
                | 28   1  2378 | 378       9       4       | 37   6  5  |
                | 9    6  378  | 378       5       78      | 4    1  2  |
                | 4    5  37   | 6         2       1       | 9    8  37 |
                :--------------+---------------------------+------------:
                | 56   3  1    | 9         678     5678    | 68   2  4  |
                | 26   8  9    | 24        46      3       | 5    7  1  |
                | 7    4  256  | 258       1       2568    | 368  9  38 |
                '--------------'---------------------------'------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    // Row
    if (!test(solver::LockedCandType::Type2Claiming,
              "1.63.548.3.54.8.1.984.1.3.55178...4..4815.........41584.258.6.175194683286.2.15.4",
              R"(
                .------------.-----------------------.---------------.
                | 1   27  6  | 3      2(-7)9   5     | 4    8   79   |
                | 3   27  5  | 4      26(-7)9  8     | 279  1   679  |
                | 9   8   4  | 6(*7)  1        2(*7) | 3    26  5    |
                :------------+-----------------------+---------------:
                | 5   1   7  | 8      236      239   | 29   4   369  |
                | 26  4   8  | 1      5        239   | 279  26  3679 |
                | 26  39  39 | 67     267      4     | 1    5   8    |
                :------------+-----------------------+---------------:
                | 4   39  2  | 5      8        37    | 6    79  1    |
                | 7   5   1  | 9      4        6     | 8    3   2    |
                | 8   6   39 | 2      37       1     | 5    79  4    |
                '------------'-----------------------'---------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    // Row
    if (!test(solver::LockedCandType::Type2Claiming,
              "....359.47...61....8529.......3.9416....1....9.16.2......9.382....12...32375.....",
              R"(
                .---------------------------.---------------.-------------------.
                | 16       126      26      | 78  3     5   | 9     78    4     |
                | 7        49       349     | 48  6     1   | 235   358   258   |
                | 34       8        5       | 2   9     47  | 1367  367   17    |
                :---------------------------+---------------+-------------------:
                | 58       257      28      | 3   578   9   | 4     1     6     |
                | 34568    4567     3468    | 47  1     478 | 257   5789  25789 |
                | 9        457      1       | 6   4578  2   | 357   3578  578   |
                :---------------------------+---------------+-------------------:
                | 145(*6)  145(*6)  4(*6)   | 9   47    3   | 8     2     57    |
                | 45(-6)8  45(-6)9  4(-6)89 | 1   2     467 | 567   4567  3     |
                | 2        3        7       | 5   48    468 | 16    469   19    |
                '---------------------------'---------------'-------------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type2Claiming,
              ".89..52....46.8.91....9...88.3947.1.41.852.39.9.3168.49.8...1..7315894...427.198.",
              R"(
                .-----------------.--------------.---------------------.
                | 136    8     9  | 14   37   5  | 2      467   67     |
                | 235    257   4  | 6    237  8  | 357    9     1      |
                | 12356  2567  56 | 124  9    34 | 37     4567  8      |
                :-----------------+--------------+---------------------:
                | 8      256   3  | 9    4    7  | 5(*6)  1     25(-6) |
                | 4      1     67 | 8    5    2  | (*6)7  3     9      |
                | 25     9     57 | 3    1    6  | 8      257   4      |
                :-----------------+--------------+---------------------:
                | 9      56    8  | 24   236  34 | 1      57    357    |
                | 7      3     1  | 5    8    9  | 4      26    26     |
                | 56     4     2  | 7    36   1  | 9      8     35     |
                '-----------------'--------------'---------------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type2Claiming,
              ".76..3.2952.9.6137..927.86..5..67.......3.......82..5...2754691641392...795618243",
              R"(
                .---------------------------.--------------.-----------------.
                | 148        7       6      | 15   48  3   | 45    2    9    |
                | 5          2       48     | 9    48  6   | 1     3    7    |
                | 134        13      9      | 2    7   15  | 8     6    45   |
                :---------------------------+--------------+-----------------:
                | 12(-3)489  5       (*3)48 | 14   6   7   | 349   18   248  |
                | 12489      168     478    | 145  3   159 | 479   178  2468 |
                | 1(-3)49    1(-3)6  (*3)47 | 8    2   19  | 3479  5    46   |
                :---------------------------+--------------+-----------------:
                | 38         38      2      | 7    5   4   | 6     9    1    |
                | 6          4       1      | 3    9   2   | 57    78   58   |
                | 7          9       5      | 6    1   8   | 2     4    3    |
                '---------------------------'--------------'-----------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type2Claiming,
              "548.263...3..8.2466.239485...346..8286.2.9..442.85.16.2..648...1.6...428384..26.5",
              R"(
                .--------------------.---------------.-----------------.
                | 5   4       8      | 17   2    6   | 3    179   179  |
                | 79  3       179    | 157  8    157 | 2    4     6    |
                | 6   17      2      | 3    9    4   | 8    5     17   |
                :--------------------+---------------+-----------------:
                | 79  15      3      | 4    6    17  | 579  8     2    |
                | 8   6       15     | 2    137  9   | 57   37    4    |
                | 4   2       79     | 8    5    37  | 1    6     379  |
                :--------------------+---------------+-----------------:
                | 2   57(*9)  57(-9) | 6    4    8   | 79   1379  1379 |
                | 1   7(*9)   6      | 579  37   357 | 4    2     8    |
                | 3   8       4      | 179  17   2   | 6    79    5    |
                '--------------------'---------------'-----------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type2Claiming,
              "1.2......35..14.92.4.52.13.239467518687195423514832.6...3.51.8.861.4......5...3.1",
              R"(
                .--------------.----------------.-------------------------.
                | 1    79   2  | 3679  78  3689 | 678      45     45      |
                | 3    5    68 | 67    1   4    | 678      9      2       |
                | 79   4    68 | 5     2   689  | 1        3      67      |
                :--------------+----------------+-------------------------:
                | 2    3    9  | 4     6   7    | 5        1      8       |
                | 6    8    7  | 1     9   5    | 4        2      3       |
                | 5    1    4  | 8     3   2    | 79       6      79      |
                :--------------+----------------+-------------------------:
                | 479  279  3  | 279   5   1    | 26(-7)9  8      46(-7)9 |
                | 8    6    1  | 2379  4   39   | 2(-7)9   5(*7)  5(-7)9  |
                | 479  279  5  | 2679  78  689  | 3        4(*7)  1       |
                '--------------'----------------'-------------------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    // Column
    if (!test(solver::LockedCandType::Type2Claiming,
              "34..9.....86..5.9797...6..4439.2.......5.3..9.58.192.38..9...5.59.8..6......5.978",
              R"(
                .------------------.----------------------------.----------------.
                | 3     4    125   | 127       9        1278    | 158   1268  56 |
                | 12    8    6     | 1234      34       5       | 13    9     7  |
                | 9     7    125   | 123       38       6       | 1358  1238  4  |
                :------------------+----------------------------+----------------:
                | 4     3    9     | 67        2        78      | 1578  168   56 |
                | 1267  126  12    | 5         4678     3       | 478   468   9  |
                | 67    5    8     | 467       1        9       | 2     46    3  |
                :------------------+----------------------------+----------------:
                | 8     126  12347 | 9         3(-4)67  12(*4)7 | 34    5     12 |
                | 5     9    12347 | 8         3(-4)7   12(*4)7 | 6     34    12 |
                | 126   126  1234  | 123(-4)6  5        12(*4)  | 9     7     8  |
                '------------------'----------------------------'----------------'
              )",
              solver::Technique::LockedCandidatesType2))
        return failed();

    return passed();
}