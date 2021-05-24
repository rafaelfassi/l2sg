#ifndef L2SG_SOLVER_H
#define L2SG_SOLVER_H

#include "Grid.h"
#include <unordered_set>

namespace l2sg
{

namespace solver
{

using TechniqueSet = std::unordered_set<solver::Technique>;

namespace techniques
{

bool nakedSingles(Grid &pGrid, Logs *logs = nullptr, bool *check = nullptr);
bool hiddenSingles(Grid &pGrid, Logs *logs = nullptr);
bool lockedCandidates(Grid &pGrid, LockedCandType lcType, Logs *logs = nullptr);
bool xWings(Grid &pGrid, Logs *logs = nullptr); // basicFish with size=2 does the same job, but this is more efficient.
bool xyWing(Grid &pGrid, Logs *logs = nullptr);
bool wWing(Grid &pGrid, Logs *logs = nullptr);
bool basicFish(Grid &pGrid, BasicFishType fishType, Logs *logs = nullptr);
bool hiddenMulti(Grid &pGrid, HiddenMultiType multiType, Logs *logs = nullptr);
bool nakedMulti(Grid &pGrid, NakedMultiType multiType, Logs *logs = nullptr);
bool skyscraper(Grid &pGrid, Logs *logs = nullptr);
bool twoStringKite(Grid &pGrid, Logs *logs = nullptr);
bool simpleGuess(Grid &pGrid, Logs *logs = nullptr);
int bruteForce(Grid &pGrid, int maxSolutions = 1, Logs *logs = nullptr);

} // namespace techniques

void getTechniquesForLevel(Level level, TechniqueSet &techniques);
Level solve(Grid &pGrid, Logs *logs = nullptr, Level maxLevel = Level::LEV_3_GUESS);
bool solveByTechniques(Grid &pGrid, const TechniqueSet &allowedTechniques, TechniqueSet *usedTechniques = nullptr,
                       Logs *logs = nullptr);

} // namespace solver

} // namespace l2sg

#endif // L2SG_SOLVER_H
