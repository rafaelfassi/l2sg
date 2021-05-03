#include "Test.h"

bool test(int maxMultiplicity, const std::string &puzzle, const std::string &board, const std::string &expectedBoard)
{
    Grid grid;
    grid.fillFromSolutionGrid(board);

    solver::techniques::nakedMulti(grid, 2, maxMultiplicity);

    return checkAll(puzzle, grid, board, expectedBoard);
}

int main(int, char **)
{
    // Naked Pair in row for values 3, 8 at:
    // (7,1) (7,5)
    if (!test(2, "..2.14..6.1356.8.4..69...1.5682...4..276.15...9...526..7...64..2.9...6..6..72....",
              R"(
                .---------------.------------------.---------------------.
                | 789  58   2   | 38    1     4    | 379   3579   6      |
                | 79   1    3   | 5     6     27   | 8     279    4      |
                | 478  458  6   | 9     378   2378 | 37    1      2357   |
                :---------------+------------------+---------------------:
                | 5    6    8   | 2     379   379  | 1379  4      1379   |
                | 34   2    7   | 6     3489  1    | 5     389    389    |
                | 134  9    14  | 348   3478  5    | 2     6      378    |
                :---------------+------------------+---------------------:
                | 18   7    15  | 138   3589  6    | 4     23589  123589 |
                | 2    38   9   | 1348  3458  38   | 6     3578   13578  |
                | 6    348  145 | 7     2     389  | 139   3589   13589  |
                '---------------'------------------'---------------------'
              )",
              R"(
                .---------------.-----------------.---------------------.
                | 789  58   2   | 38   1     4    | 379   3579   6      |
                | 79   1    3   | 5    6     27   | 8     279    4      |
                | 478  458  6   | 9    378   2378 | 37    1      2357   |
                :---------------+-----------------+---------------------:
                | 5    6    8   | 2    379   379  | 1379  4      1379   |
                | 34   2    7   | 6    3489  1    | 5     389    389    |
                | 134  9    14  | 348  3478  5    | 2     6      378    |
                :---------------+-----------------+---------------------:
                | 18   7    15  | 138  3589  6    | 4     23589  123589 |
                | 2    38   9   | 14   45    38   | 6     57     157    |
                | 6    348  145 | 7    2     389  | 139   3589   13589  |
                '---------------'-----------------'---------------------'
              )"))
        return failed();

    // Naked Pair in column for values 5, 6 at:
    // (3,5)
    // (8,5)
    if (!test(2, "5...27.3.7..3...1...65.1729...2...872751..643.6.7.42..64.9.23...5...3..2...81....",
              R"(
                .-----------------.---------------.-----------------.
                | 5     189  149  | 46  2     7   | 48   3     468  |
                | 7     28   24   | 3   4689  689 | 458  1     4568 |
                | 34    38   6    | 5   48    1   | 7    2     9    |
                :-----------------+---------------+-----------------:
                | 1349  139  1349 | 2   356   56  | 159  8     7    |
                | 2     7    5    | 1   89    89  | 6    4     3    |
                | 1389  6    1389 | 7   35    4   | 2    59    15   |
                :-----------------+---------------+-----------------:
                | 6     4    18   | 9   57    2   | 3    57    18   |
                | 189   5    1789 | 46  467   3   | 189  679   2    |
                | 39    239  2379 | 8   1     56  | 459  5679  45   |
                '-----------------'---------------'-----------------'
              )",
              R"(
                .-----------------.--------------.-----------------.
                | 5     189  149  | 46  2     7  | 48   3     468  |
                | 7     28   24   | 3   4689  89 | 458  1     4568 |
                | 34    38   6    | 5   48    1  | 7    2     9    |
                :-----------------+--------------+-----------------:
                | 1349  139  1349 | 2   356   56 | 159  8     7    |
                | 2     7    5    | 1   89    89 | 6    4     3    |
                | 1389  6    1389 | 7   35    4  | 2    59    15   |
                :-----------------+--------------+-----------------:
                | 6     4    18   | 9   57    2  | 3    57    18   |
                | 189   5    1789 | 46  467   3  | 189  679   2    |
                | 39    239  2379 | 8   1     56 | 459  5679  45   |
                '-----------------'--------------'-----------------'
              )"))
        return failed();

    // Naked Pair in block for values 2, 8 at:
    // ..... (3,8)
    // (4,6) .....
    if (!test(2, "6215..4..7.9..2..65.8......4.7...93...5.7..64.63.....1156...3898921..647374..9125",
              R"(
                .-----------.---------------------.--------------.
                | 6   2   1 | 5      389    378   | 4    79   38 |
                | 7   34  9 | 348    1348   2     | 58   15   6  |
                | 5   34  8 | 34679  13469  13467 | 27   179  23 |
                :-----------+---------------------+--------------:
                | 4   18  7 | 26     1256   156   | 9    3    28 |
                | 29  18  5 | 239    7      13    | 28   6    4  |
                | 29  6   3 | 2489   2489   48    | 257  57   1  |
                :-----------+---------------------+--------------:
                | 1   5   6 | 247    24     47    | 3    8    9  |
                | 8   9   2 | 1      35     35    | 6    4    7  |
                | 3   7   4 | 68     68     9     | 1    2    5  |
                '-----------'---------------------'--------------'
              )",
              R"(
                .-----------.---------------------.-------------.
                | 6   2   1 | 5      389    378   | 4   79   38 |
                | 7   34  9 | 348    1348   2     | 58  15   6  |
                | 5   34  8 | 34679  13469  13467 | 27  179  23 |
                :-----------+---------------------+-------------:
                | 4   18  7 | 26     1256   156   | 9   3    28 |
                | 29  18  5 | 239    7      13    | 28  6    4  |
                | 29  6   3 | 2489   2489   48    | 57  57   1  |
                :-----------+---------------------+-------------:
                | 1   5   6 | 247    24     47    | 3   8    9  |
                | 8   9   2 | 1      35     35    | 6   4    7  |
                | 3   7   4 | 68     68     9     | 1   2    5  |
                '-----------'---------------------'-------------'

              )"))
        return failed();

    // Naked Triple in row for values 2, 5, 9 at:
    // (1,2) (1,7) (1,8)
    if (!test(3, "..869.137...3148...318.7.6.8172.6...3....8.16...13.728.934..68...658....18..632..",
              R"(
                .------------------.--------------.------------------.
                | 245    245   8   | 6   9    25  | 1     3     7    |
                | 25679  2567  259 | 3   1    4   | 8     59    259  |
                | 259    3     1   | 8   25   7   | 459   6     2459 |
                :------------------+--------------+------------------:
                | 8      1     7   | 2   45   6   | 3459  459   3459 |
                | 3      25    259 | 79  457  8   | 45    1     6    |
                | 4569   456   459 | 1   3    59  | 7     2     8    |
                :------------------+--------------+------------------:
                | 257    9     3   | 4   27   12  | 6     8     15   |
                | 247    247   6   | 5   8    129 | 349   479   1349 |
                | 1      8     45  | 79  6    3   | 2     4579  459  |
                '------------------'--------------'------------------'
              )",
              R"(
                .----------------.--------------.------------------.
                | 245   245  8   | 6   9    25  | 1     3     7    |
                | 67    67   259 | 3   1    4   | 8     59    259  |
                | 259   3    1   | 8   25   7   | 459   6     2459 |
                :----------------+--------------+------------------:
                | 8     1    7   | 2   45   6   | 3459  459   3459 |
                | 3     25   259 | 79  457  8   | 45    1     6    |
                | 4569  456  459 | 1   3    59  | 7     2     8    |
                :----------------+--------------+------------------:
                | 257   9    3   | 4   27   12  | 6     8     15   |
                | 247   247  6   | 5   8    129 | 349   479   1349 |
                | 1     8    45  | 79  6    3   | 2     4579  459  |
                '----------------'--------------'------------------'
              )"))
        return failed();

    // Naked Triple in column for values 1, 4, 7 at:
    // (0,4)
    // (7,4)
    // (8,4)
    if (!test(3, ".25..69..6......12....32..6....6.24.1.2.....3.6328....7..529....5.....292.96..875",
              R"(
                .------------------.--------------------.-----------------.
                | 348  2      5    | 1478   147   6     | 9     38    478 |
                | 6    34789  478  | 4789   4579  4578  | 347   1     2   |
                | 489  14789  1478 | 4789   3     2     | 457   58    6   |
                :------------------+--------------------+-----------------:
                | 589  789    78   | 1379   6     1357  | 2     4     178 |
                | 1    4789   2    | 479    4579  457   | 567   5689  3   |
                | 459  6      3    | 2      8     1457  | 157   59    17  |
                :------------------+--------------------+-----------------:
                | 7    1348   1468 | 5      2     9     | 1346  36    14  |
                | 34   5      146  | 13478  147   13478 | 1346  2     9   |
                | 2    134    9    | 6      14    134   | 8     7     5   |
                '------------------'--------------------'-----------------'
              )",
              R"(
                .------------------.-------------------.-----------------.
                | 348  2      5    | 1478   147  6     | 9     38    478 |
                | 6    34789  478  | 4789   59   4578  | 347   1     2   |
                | 489  14789  1478 | 4789   3    2     | 457   58    6   |
                :------------------+-------------------+-----------------:
                | 589  789    78   | 1379   6    1357  | 2     4     178 |
                | 1    4789   2    | 479    59   457   | 567   5689  3   |
                | 459  6      3    | 2      8    1457  | 157   59    17  |
                :------------------+-------------------+-----------------:
                | 7    1348   1468 | 5      2    9     | 1346  36    14  |
                | 34   5      146  | 13478  147  13478 | 1346  2     9   |
                | 2    134    9    | 6      14   134   | 8     7     5   |
                '------------------'-------------------'-----------------'
              )"))
        return failed();

    // Naked Triple in block for values 1, 2, 5 at:
    // ..... (1,4) (1,5)
    // (2,3) ..... .....
    if (!test(3, "6.....4..7.34...96..9.863.7.6....7.18...6.9.49.7....6.2.697.543394625178.7....629",
              R"(
                .-----------------.--------------------.-------------.
                | 6    1258  1258 | 12357  1359  12379 | 4   158  25 |
                | 7    1258  3    | 4      15    12    | 28  9    6  |
                | 145  1245  9    | 125    8     6     | 3   15   7  |
                :-----------------+--------------------+-------------:
                | 45   6     25   | 2358   3459  23489 | 7   358  1  |
                | 8    1235  125  | 12357  6     1237  | 9   35   4  |
                | 9    1345  7    | 1358   1345  1348  | 28  6    25 |
                :-----------------+--------------------+-------------:
                | 2    18    6    | 9      7     18    | 5   4    3  |
                | 3    9     4    | 6      2     5     | 1   7    8  |
                | 15   7     158  | 138    134   1348  | 6   2    9  |
                '-----------------'--------------------'-------------'
              )",
              R"(
                .-----------------.--------------------.-------------.
                | 6    1258  1258 | 37     39    379   | 4   158  25 |
                | 7    1258  3    | 4      15    12    | 28  9    6  |
                | 145  1245  9    | 125    8     6     | 3   15   7  |
                :-----------------+--------------------+-------------:
                | 45   6     25   | 2358   3459  23489 | 7   358  1  |
                | 8    1235  125  | 12357  6     1237  | 9   35   4  |
                | 9    1345  7    | 1358   1345  1348  | 28  6    25 |
                :-----------------+--------------------+-------------:
                | 2    18    6    | 9      7     18    | 5   4    3  |
                | 3    9     4    | 6      2     5     | 1   7    8  |
                | 15   7     158  | 138    134   1348  | 6   2    9  |
                '-----------------'--------------------'-------------'
              )"))
        return failed();

    return passed();
}