#include "Test.h"

bool test(const std::string &puzzle, const std::string &board, solver::Technique expectedTechnique)
{
    Grid grid;
    grid.fillBoard(board);

    solver::Logs logs;
    bool changed = solver::techniques::twoStringKite(grid, &logs);

    return checkAll(puzzle, grid, board, changed, expectedTechnique, logs);
}

int main(int, char **)
{
    if (!test("347.6.9.825.4.93.7..1..34.2689..4231735.2.6491249368754.23..186..36..5245...4.793",
              R"(
                .-------------------.------------------.----------.
                | 3      4    7     | 125  6       125 | 9  15  8 |
                | 2      5    (^8)6 | 4    (*8)1   9   | 3  16  7 |
                | (^8)9  69   1     | 578  578     3   | 4  56  2 |
                :-------------------+------------------+----------:
                | 6      8    9     | 57   57      4   | 2  3   1 |
                | 7      3    5     | 18   2       18  | 6  4   9 |
                | 1      2    4     | 9    3       6   | 8  7   5 |
                :-------------------+------------------+----------:
                | 4      79   2     | 3    59      57  | 1  8   6 |
                | (*8)9  179  3     | 6    (-8)19  178 | 5  2   4 |
                | 5      16   68    | 128  4       128 | 7  9   3 |
                '-------------------'------------------'----------'
              )",
              solver::Technique::TwoStringKite))
        return failed();

    if (!test("56417..2.387.2.16.....6347..5.34.71..4275193.173.96...7.561........8...7.98.376.1",
              R"(
                .--------------.-----------------.------------------.
                | 5    6    4  | 1      7  (^8)9 | 38   2    389    |
                | 3    8    7  | 459    2  459   | 1    6    59     |
                | 29   12   19 | (^8)5  6  3     | 4    7    (*8)5  |
                :--------------+-----------------+------------------:
                | 689  5    69 | 3      4  (*8)2 | 7    1    (-8)26 |
                | 68   4    2  | 7      5  1     | 9    3    68     |
                | 1    7    3  | 28     9  6     | 258  458  248    |
                :--------------+-----------------+------------------:
                | 7    23   5  | 6      1  249   | 238  489  2348   |
                | 246  123  16 | 2459   8  2459  | 235  459  7      |
                | 24   9    8  | 245    3  7     | 6    45   1      |
                '--------------'-----------------'------------------'
              )",
              solver::Technique::TwoStringKite))
        return failed();

    if (!test("5.9..8.4.3..6..5......5...7...9164584.68753.2.5.342...6.5........4..1..5.2.5..9.4",
              R"(
                .------------------.--------------------.---------------------.
                | 5    167    9    | 17   23    8       | 126   4      (^3)16 |
                | 3    1478   1278 | 6    29    47      | 5     1289   19     |
                | 128  1468   128  | 14   5     (*3)9   | 1268  (^3)9  7      |
                :------------------+--------------------+---------------------:
                | 27   37     237  | 9    1     6       | 4     5      8      |
                | 4    19     6    | 8    7     5       | 3     19     2      |
                | 189  5      18   | 3    4     2       | 167   1679   169    |
                :------------------+--------------------+---------------------:
                | 6    13789  5    | 247  389   (-3)479 | 1278  12378  (*3)1  |
                | 789  3789   4    | 27   3689  1       | 2678  23678  5      |
                | 178  2      1378 | 5    368   37      | 9     13678  4      |
                '------------------'--------------------'---------------------'
              )",
              solver::Technique::TwoStringKite))
        return failed();

    if (!test("37.6.....54...1....1643...91....47..7.42.3..8..51.....621389457.53742.614.7516.32",
              R"(
                .---------------------.--------------.------------------.
                | 3       7     289   | 6   29   58  | 125  1248    45  |
                | 5       4     289   | 89  279  1   | 36   278     36  |
                | (*2)8   1     6     | 4   3    578 | 25   (-2)78  9   |
                :---------------------+--------------+------------------:
                | 1       36    (^2)8 | 89  56   4   | 7    (*2)9   356 |
                | 7       69    4     | 2   569  3   | 156  19      8   |
                | (^2)89  3689  5     | 1   679  78  | 236  249     346 |
                :---------------------+--------------+------------------:
                | 6       2     1     | 3   8    9   | 4    5       7   |
                | 89      5     3     | 7   4    2   | 89   6       1   |
                | 4       89    7     | 5   1    6   | 89   3       2   |
                '---------------------'--------------'------------------'

              )",
              solver::Technique::TwoStringKite))
        return failed();

    if (!test("3587.9..6..48..9.5.924.58.7.17.536.88...475.25.968..73....9.7..9.....2....15.8369",
              R"(
                .-------------------.---------------------.-------------.
                | 3    5        8   | 7       12    9     | 14  124  6  |
                | 167  67       4   | 8       1236  126   | 9   123  5  |
                | 16   9        2   | 4       136   5     | 8   13   7  |
                :-------------------+---------------------+-------------:
                | 24   1        7   | (^2)9   5     3     | 6   49   8  |
                | 8    36       36  | 19      4     7     | 5   19   2  |
                | 5    (*2)4    9   | 6       8     (^2)1 | 14  7    3  |
                :-------------------+---------------------+-------------:
                | 26   (-2)368  356 | (*2)13  9     1246  | 7   58   14 |
                | 9    3678     356 | 13      167   146   | 2   58   14 |
                | 247  247      1   | 5       27    8     | 3   6    9  |
                '-------------------'---------------------'-------------'
              )",
              solver::Technique::TwoStringKite))
        return failed();

    if (!test("657.9...3832.7...549153872631896..74.748.361..267143.818.3.746.74..8..3126314...7",
              R"(
                .-----------.---------------.-------------------.
                | 6   5  7  | 24  9   12    | 18     48      3  |
                | 8   3  2  | 46  7   16    | 19     49      5  |
                | 4   9  1  | 5   3   8     | 7      2       6  |
                :-----------+---------------+-------------------:
                | 3   1  8  | 9   6   (*5)2 | (^5)2  7       4  |
                | 59  7  4  | 8   25  3     | 6      1       29 |
                | 59  2  6  | 7   1   4     | 3      (^5)9   8  |
                :-----------+---------------+-------------------:
                | 1   8  59 | 3   25  7     | 4      6       29 |
                | 7   4  59 | 26  8   2569  | 259    3       1  |
                | 2   6  3  | 1   4   (-5)9 | 589    (*5)89  7  |
                '-----------'---------------'-------------------'
              )",
              solver::Technique::TwoStringKite))
        return failed();

    if (!test("1.4.38...7.3.45...26.917.43832.649.5....29..66.958..2492.47..38....5..97...89...1",
              R"(
                .---------------------.---------------.---------------.
                | 1    59      4      | 26   3  8     | 67    567  29 |
                | 7    89      3      | 26   4  5     | 168   168  29 |
                | 2    6       58     | 9    1  7     | 58    4    3  |
                :---------------------+---------------+---------------:
                | 8    3       2      | 17   6  4     | 9     17   5  |
                | 45   45      17     | 137  2  9     | 1378  178  6  |
                | 6    (*1)7   9      | 5    8  (-1)3 | 137   2    4  |
                :---------------------+---------------+---------------:
                | 9    2       (^1)56 | 4    7  (*1)6 | 56    3    8  |
                | 34   (^1)48  168    | 13   5  1236  | 24    9    7  |
                | 345  457     67     | 8    9  236   | 24    56   1  |
                '---------------------'---------------'---------------'
              )",
              solver::Technique::TwoStringKite))
        return failed();

    if (!test("5..27...96.7.1.2....23..17...612...72758634918..79.6.2.29.817..15863792476.9.2.18",
              R"(
                .------------------.-------------------.-------------.
                | 5      1348  134 | 2      7      68  | 38  468  9  |
                | 6      3489  7   | 45     1      89  | 2   48   35 |
                | (-4)9  489   2   | 3      (*4)5  689 | 1   7    56 |
                :------------------+-------------------+-------------:
                | 349    349   6   | 1      2      45  | 58  358  7  |
                | 2      7     5   | 8      6      3   | 4   9    1  |
                | 8      134   134 | 7      9      45  | 6   35   2  |
                :------------------+-------------------+-------------:
                | (*4)3  2     9   | (^4)5  8      1   | 7   56   36 |
                | 1      5     8   | 6      3      7   | 9   2    4  |
                | 7      6     34  | 9      (^4)5  2   | 35  1    8  |
                '------------------'-------------------'-------------'
              )",
              solver::Technique::TwoStringKite))
        return failed();

    if (!test("...2136...3.7659....69483.54..3.6.9...14897...6.1.7..46.58724....3694.5..485312..",
              R"(
                .--------------------.----------.--------------------.
                | (-7)589  5789  479 | 2  1   3 | 6    478    (*7)8  |
                | 18       3     24  | 7  6   5 | 9    148    128    |
                | 127      127   6   | 9  4   8 | 3    127    5      |
                :--------------------+----------+--------------------:
                | 4        78    27  | 3  25  6 | 158  9      128    |
                | 235      25    1   | 4  8   9 | 7    236    36     |
                | 389      6     29  | 1  25  7 | 58   38     4      |
                :--------------------+----------+--------------------:
                | 6        19    5   | 8  7   2 | 4    13     39     |
                | 127      127   3   | 6  9   4 | 18   5      (^7)18 |
                | (*7)9    4     8   | 5  3   1 | 2    (^7)6  69     |
                '--------------------'----------'--------------------'
              )",
              solver::Technique::TwoStringKite))
        return failed();

    return passed();
}