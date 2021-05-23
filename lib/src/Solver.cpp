#include "Solver.h"

namespace l2sg::solver
{

static const TechniqueSet techsLevel0 = {
    //
    solver::Technique::NakedSingles, solver::Technique::HiddenSingles
    //
};

static const TechniqueSet techsLevel1 = {
    //
    solver::Technique::NakedPair,
    solver::Technique::NakedTriple,
    solver::Technique::HiddenPair,
    solver::Technique::HiddenTriple,
    solver::Technique::LockedCandidatesType1,
    solver::Technique::LockedCandidatesType2
    //
};

static const TechniqueSet techsLevel2 = {
    //
    solver::Technique::NakedQuadruple, solver::Technique::HiddenQuadruple,
    solver::Technique::XWings,         solver::Technique::Swordfish,
    solver::Technique::Jellyfish,      solver::Technique::XYWing,
    solver::Technique::WWing,          solver::Technique::Skyscraper,
    solver::Technique::TwoStringKite
    //
};

static const TechniqueSet techsLevel3 = {
    //
    solver::Technique::SimpleGuess, solver::Technique::BruteForce
    //
};

void getTechniquesForLevel(Level level, TechniqueSet &techniques)
{
    switch (level)
    {
    case LEV_0_LOGIC:
        techniques.insert(techsLevel0.begin(), techsLevel0.end());
        break;
    case LEV_1_LOGIC:
        techniques.insert(techsLevel1.begin(), techsLevel1.end());
        break;
    case LEV_2_LOGIC:
        techniques.insert(techsLevel2.begin(), techsLevel2.end());
        break;
    case LEV_3_GUESS:
        techniques.insert(techsLevel3.begin(), techsLevel3.end());
        break;
    default:
        break;
    }
}

Level solve(Grid &pGrid, Logs *logs, Level maxLevel)
{
    static const std::vector<Level> levelsLists = {
        LEV_0_LOGIC,
        LEV_1_LOGIC,
        LEV_2_LOGIC,
        LEV_3_GUESS
    };

    TechniqueSet techniques;
    techniques.reserve(30);
    for (size_t i = 0; i < levelsLists.size(); ++i)
    {
        const auto level = levelsLists.at(i);
        getTechniquesForLevel(level, techniques);
        if (solveByTechniques(pGrid, techniques, nullptr, logs))
            return level;
    }

    return LEV_UNKNOWN;
}

bool solveByTechniques(Grid &pGrid, const TechniqueSet &allowedTechniques, TechniqueSet *usedTechniques, Logs *logs)
{
#define EXEC_IF(p_technique, p_func)                                                                                   \
    if (allowedTechniques.find(p_technique) != allowedTechniques.end())                                                \
    {                                                                                                                  \
        if (p_func)                                                                                                    \
        {                                                                                                              \
            if (usedTechniques)                                                                                        \
            {                                                                                                          \
                usedTechniques->insert(p_technique);                                                                   \
            }                                                                                                          \
            continue;                                                                                                  \
        }                                                                                                              \
    }

    while (true)
    {
        EXEC_IF(solver::Technique::NakedSingles, solver::techniques::nakedSingles(pGrid, logs));

        if (pGrid.isFull())
            return true;

        EXEC_IF(solver::Technique::HiddenSingles, solver::techniques::hiddenSingles(pGrid, logs));

        EXEC_IF(solver::Technique::LockedCandidatesType1,
                solver::techniques::lockedCandidates(pGrid, LockedCandidatesType::Type1Pointing, logs));

        EXEC_IF(solver::Technique::LockedCandidatesType2,
                solver::techniques::lockedCandidates(pGrid, LockedCandidatesType::Type2Claiming, logs));

        EXEC_IF(solver::Technique::NakedPair,
                solver::techniques::nakedMulti(pGrid, solver::NakedMultiType::Pair, logs));

        EXEC_IF(solver::Technique::HiddenPair,
                solver::techniques::hiddenMulti(pGrid, solver::HiddenMultiType::Pair, logs));

        EXEC_IF(solver::Technique::NakedTriple,
                solver::techniques::nakedMulti(pGrid, solver::NakedMultiType::Triple, logs));

        EXEC_IF(solver::Technique::HiddenTriple,
                solver::techniques::hiddenMulti(pGrid, solver::HiddenMultiType::Triple, logs));

        EXEC_IF(solver::Technique::NakedQuadruple,
                solver::techniques::nakedMulti(pGrid, solver::NakedMultiType::Quadruple, logs));

        EXEC_IF(solver::Technique::HiddenQuadruple,
                solver::techniques::hiddenMulti(pGrid, solver::HiddenMultiType::Quadruple, logs));

        EXEC_IF(solver::Technique::XWings, solver::techniques::xWings(pGrid, logs));

        EXEC_IF(solver::Technique::Skyscraper, solver::techniques::skyscraper(pGrid, logs));

        EXEC_IF(solver::Technique::TwoStringKite, solver::techniques::twoStringKite(pGrid, logs));

        EXEC_IF(solver::Technique::XYWing, solver::techniques::xyWing(pGrid, logs));

        EXEC_IF(solver::Technique::WWing, solver::techniques::wWing(pGrid, logs));

        EXEC_IF(solver::Technique::Swordfish,
                solver::techniques::basicFish(pGrid, solver::BasicFishType::Swordfish, logs));

        EXEC_IF(solver::Technique::Jellyfish,
                solver::techniques::basicFish(pGrid, solver::BasicFishType::Jellyfish, logs));

        EXEC_IF(solver::Technique::SimpleGuess, solver::techniques::simpleGuess(pGrid, logs));

        EXEC_IF(solver::Technique::BruteForce, (techniques::bruteForce(pGrid, 1, logs) == 1));

        return false;
    }
}

} // namespace l2sg::solver