#include "Solver.h"

#define SWITCH_LEVEL(p_newLevel, p_level, p_maxLevel)                                                                  \
    if (p_newLevel > p_maxLevel)                                                                                       \
    {                                                                                                                  \
        p_level = LEV_UNKNOWN;                                                                                         \
        break;                                                                                                         \
    }                                                                                                                  \
    if (p_level < p_newLevel)                                                                                          \
        p_level = p_newLevel;

namespace sudoku
{

Level solver::solve(Grid &pGrid, Logs *logs, Level maxLevel)
{
    int level(LEV_0_LOGIC);

    while (true)
    {
        techniques::nakedSingles(pGrid, logs);

        if (pGrid.isFull())
            break;

        if (techniques::hiddenSingles(pGrid, logs))
            continue;

        SWITCH_LEVEL(LEV_1_LOGIC, level, maxLevel)

        if (techniques::lockedCandidates(pGrid, logs))
            continue;

        if (techniques::nakedMulti(pGrid, NakedMultiType::Pair, logs))
            continue;

        if (techniques::hiddenMulti(pGrid, HiddenMultiType::Pair, logs))
            continue;

        if (techniques::nakedMulti(pGrid, NakedMultiType::Triple, logs))
            continue;

        if (techniques::hiddenMulti(pGrid, HiddenMultiType::Triple, logs))
            continue;

        SWITCH_LEVEL(LEV_2_LOGIC, level, maxLevel)

        if (techniques::nakedMulti(pGrid, NakedMultiType::Quadruple, logs))
            continue;

        if (techniques::hiddenMulti(pGrid, HiddenMultiType::Quadruple, logs))
            continue;

        if (techniques::xWings(pGrid, logs))
            continue;

        if (techniques::skyscraper(pGrid, logs))
            continue;

        if (techniques::twoStringKite(pGrid, logs))
            continue;

        if (techniques::xyWing(pGrid, logs))
            continue;

        if (techniques::wWing(pGrid, logs))
            continue;

        if (techniques::basicFish(pGrid, solver::BasicFishType::Swordfish, logs))
            continue;

        if (techniques::basicFish(pGrid, solver::BasicFishType::Jellyfish, logs))
            continue;

        SWITCH_LEVEL(LEV_3_GUESS, level, maxLevel)

        if(solver::techniques::simpleGuess(pGrid, logs))
            continue;

        if (techniques::bruteForce(pGrid, 1, logs) == 1)
            break;

        level = LEV_UNKNOWN;
        break;
    }

    return static_cast<Level>(level);
}

} // namespace sudoku