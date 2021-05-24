#include "Solver.h"

namespace l2sg::solver
{

static const std::vector<std::tuple<Level, Technique, std::function<bool(Grid &, Logs *)>>> g_techniquesVec = {
    //
    {Level::LEV_0_LOGIC, solver::Technique::NakedSingles,
     [](Grid &pGrid, Logs *logs) { return techniques::nakedSingles(pGrid, logs); }},
    //
    {Level::LEV_0_LOGIC, solver::Technique::HiddenSingles,
     [](Grid &pGrid, Logs *logs) { return techniques::hiddenSingles(pGrid, logs); }},
    //
    {Level::LEV_1_LOGIC, solver::Technique::LockedCandidatesType1,
     [](Grid &pGrid, Logs *logs) { return techniques::lockedCandidates(pGrid, LockedCandType::Type1Pointing, logs); }},
    //
    {Level::LEV_1_LOGIC, solver::Technique::LockedCandidatesType2,
     [](Grid &pGrid, Logs *logs) { return techniques::lockedCandidates(pGrid, LockedCandType::Type2Claiming, logs); }},
    //
    {Level::LEV_1_LOGIC, solver::Technique::NakedPair,
     [](Grid &pGrid, Logs *logs) { return techniques::nakedMulti(pGrid, NakedMultiType::Pair, logs); }},
    //
    {Level::LEV_1_LOGIC, solver::Technique::HiddenPair,
     [](Grid &pGrid, Logs *logs) { return techniques::hiddenMulti(pGrid, HiddenMultiType::Pair, logs); }},
    //
    {Level::LEV_1_LOGIC, solver::Technique::NakedTriple,
     [](Grid &pGrid, Logs *logs) { return techniques::nakedMulti(pGrid, NakedMultiType::Triple, logs); }},
    //
    {Level::LEV_1_LOGIC, solver::Technique::HiddenTriple,
     [](Grid &pGrid, Logs *logs) { return techniques::hiddenMulti(pGrid, HiddenMultiType::Triple, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::NakedQuadruple,
     [](Grid &pGrid, Logs *logs) { return techniques::nakedMulti(pGrid, NakedMultiType::Quadruple, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::HiddenQuadruple,
     [](Grid &pGrid, Logs *logs) { return techniques::hiddenMulti(pGrid, HiddenMultiType::Quadruple, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::XWings,
     [](Grid &pGrid, Logs *logs) { return techniques::xWings(pGrid, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::Skyscraper,
     [](Grid &pGrid, Logs *logs) { return techniques::skyscraper(pGrid, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::TwoStringKite,
     [](Grid &pGrid, Logs *logs) { return techniques::twoStringKite(pGrid, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::XYWing,
     [](Grid &pGrid, Logs *logs) { return techniques::xyWing(pGrid, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::WWing,
     [](Grid &pGrid, Logs *logs) { return techniques::wWing(pGrid, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::Swordfish,
     [](Grid &pGrid, Logs *logs) { return techniques::basicFish(pGrid, solver::BasicFishType::Swordfish, logs); }},
    //
    {Level::LEV_2_LOGIC, solver::Technique::Jellyfish,
     [](Grid &pGrid, Logs *logs) { return techniques::basicFish(pGrid, solver::BasicFishType::Jellyfish, logs); }},
    //
    {Level::LEV_3_GUESS, solver::Technique::SimpleGuess,
     [](Grid &pGrid, Logs *logs) { return solver::techniques::simpleGuess(pGrid, logs); }},
    //
    {Level::LEV_3_GUESS, solver::Technique::BruteForce,
     [](Grid &pGrid, Logs *logs) { return (techniques::bruteForce(pGrid, 1, logs) == 1); }}
    //
};

Level solve(Grid &pGrid, Logs *logs, Level maxLevel)
{
    Level level(Level::LEV_0_LOGIC);

    auto it = g_techniquesVec.begin();
    while (it != g_techniquesVec.end())
    {
        const auto &[techLevel, technique, solveFunc] = *it;
        if (techLevel > maxLevel)
            return Level::LEV_UNKNOWN;

        level = std::max(level, techLevel);

        if (it == g_techniquesVec.begin())
        {
            // Alway run NakedSingle before verify if the grid is full
            solveFunc(pGrid, logs);
            if (pGrid.isFull())
                return level;
        }
        else if (solveFunc(pGrid, logs))
        {
            it = g_techniquesVec.begin();
            continue;
        }
        ++it;
    }

    return Level::LEV_UNKNOWN;
}

bool solveByTechniques(Grid &pGrid, const TechniqueSet &allowedTechniques, TechniqueSet *usedTechniques, Logs *logs)
{
    auto it = g_techniquesVec.begin();
    while (it != g_techniquesVec.end())
    {
        const auto &[techLevel, technique, solveFunc] = *it;

        if (it == g_techniquesVec.begin())
        {
            // Alway run NakedSingle before verify if the grid is full
            solveFunc(pGrid, logs);
            if (pGrid.isFull())
                return true;
        }
        else if (allowedTechniques.find(technique) != allowedTechniques.end())
        {
            if (solveFunc(pGrid, logs))
            {
                if (usedTechniques)
                    usedTechniques->insert(technique);
                it = g_techniquesVec.begin();
                continue;
            }
        }
        ++it;
    }

    return false;
}

} // namespace l2sg::solver