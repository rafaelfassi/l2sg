#include "Solver.h"

namespace l2sg::solver
{

#define SWITCH_LEVEL(p_newLevel, p_level, p_maxLevel)                                                                  \
    if (p_newLevel > p_maxLevel)                                                                                       \
    {                                                                                                                  \
        p_level = LEV_UNKNOWN;                                                                                         \
        break;                                                                                                         \
    }                                                                                                                  \
    if (p_level < p_newLevel)                                                                                          \
        p_level = p_newLevel;

Level solve(Grid &pGrid, Logs *logs, Level maxLevel)
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

        if (techniques::lockedCandidates(pGrid, LockedCandidatesType::Type1Pointing, logs))
            continue;

        if (techniques::lockedCandidates(pGrid, LockedCandidatesType::Type2Claiming, logs))
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

        if (solver::techniques::simpleGuess(pGrid, logs))
            continue;

        if (techniques::bruteForce(pGrid, 1, logs) == 1)
            break;

        level = LEV_UNKNOWN;
        break;
    }

    return static_cast<Level>(level);
}

#define EXEC_IF(p_technique, p_allowedTechniques, p_usedTechniques, p_func)                                            \
    if (p_allowedTechniques.find(p_technique) != p_allowedTechniques.end())                                            \
    {                                                                                                                  \
        if (p_func)                                                                                                    \
        {                                                                                                              \
            p_usedTechniques.insert(p_technique);                                                                      \
            continue;                                                                                                  \
        }                                                                                                              \
    }

bool solveByTechniques(Grid &pGrid, const std::unordered_set<solver::Technique> &allowedTechniques,
                       std::unordered_set<solver::Technique> &usedTechniques, Logs *logs)
{
    while (true)
    {
        EXEC_IF(solver::Technique::NakedSingles, allowedTechniques, usedTechniques,
                solver::techniques::nakedSingles(pGrid, logs));

        if (pGrid.isFull())
            return true;

        EXEC_IF(solver::Technique::HiddenSingles, allowedTechniques, usedTechniques,
                solver::techniques::hiddenSingles(pGrid, logs));

        EXEC_IF(solver::Technique::LockedCandidatesType1, allowedTechniques, usedTechniques,
                solver::techniques::lockedCandidates(pGrid, LockedCandidatesType::Type1Pointing, logs));

        EXEC_IF(solver::Technique::LockedCandidatesType2, allowedTechniques, usedTechniques,
                solver::techniques::lockedCandidates(pGrid, LockedCandidatesType::Type2Claiming, logs));

        EXEC_IF(solver::Technique::NakedPair, allowedTechniques, usedTechniques,
                solver::techniques::nakedMulti(pGrid, solver::NakedMultiType::Pair, logs));

        EXEC_IF(solver::Technique::HiddenPair, allowedTechniques, usedTechniques,
                solver::techniques::hiddenMulti(pGrid, solver::HiddenMultiType::Pair, logs));

        EXEC_IF(solver::Technique::NakedTriple, allowedTechniques, usedTechniques,
                solver::techniques::nakedMulti(pGrid, solver::NakedMultiType::Triple, logs));

        EXEC_IF(solver::Technique::HiddenTriple, allowedTechniques, usedTechniques,
                solver::techniques::hiddenMulti(pGrid, solver::HiddenMultiType::Triple, logs));

        EXEC_IF(solver::Technique::NakedQuadruple, allowedTechniques, usedTechniques,
                solver::techniques::nakedMulti(pGrid, solver::NakedMultiType::Quadruple, logs));

        EXEC_IF(solver::Technique::HiddenQuadruple, allowedTechniques, usedTechniques,
                solver::techniques::hiddenMulti(pGrid, solver::HiddenMultiType::Quadruple, logs));

        EXEC_IF(solver::Technique::XWings, allowedTechniques, usedTechniques, //
                solver::techniques::xWings(pGrid, logs));

        EXEC_IF(solver::Technique::Skyscraper, allowedTechniques, usedTechniques,
                solver::techniques::skyscraper(pGrid, logs));

        EXEC_IF(solver::Technique::TwoStringKite, allowedTechniques, usedTechniques,
                solver::techniques::twoStringKite(pGrid, logs));

        EXEC_IF(solver::Technique::XYWing, allowedTechniques, usedTechniques, //
                solver::techniques::xyWing(pGrid, logs));

        EXEC_IF(solver::Technique::WWing, allowedTechniques, usedTechniques, //
                solver::techniques::wWing(pGrid, logs));

        EXEC_IF(solver::Technique::Swordfish, allowedTechniques, usedTechniques,
                solver::techniques::basicFish(pGrid, solver::BasicFishType::Swordfish, logs));

        EXEC_IF(solver::Technique::Jellyfish, allowedTechniques, usedTechniques,
                solver::techniques::basicFish(pGrid, solver::BasicFishType::Jellyfish, logs));

        EXEC_IF(solver::Technique::SimpleGuess, allowedTechniques, usedTechniques,
                solver::techniques::simpleGuess(pGrid, logs));

        EXEC_IF(solver::Technique::BruteForce, allowedTechniques, usedTechniques,
                (techniques::bruteForce(pGrid, 1, logs) == 1));

        return false;
    }
}

} // namespace l2sg::solver