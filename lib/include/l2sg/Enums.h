#ifndef L2SG_ENUMS_H
#define L2SG_ENUMS_H

#include <string>

namespace l2sg
{

namespace solver
{

enum class Technique
{
    None,
    BadPuzzle,
    NakedSingles,
    NakedPair,
    NakedTriple,
    NakedQuadruple,
    HiddenSingles,
    HiddenPair,
    HiddenTriple,
    HiddenQuadruple,
    LockedCandidatesType1,
    LockedCandidatesType2,
    XWings,
    Swordfish,
    Jellyfish,
    XYWing,
    WWing,
    Skyscraper,
    TwoStringKite,
    SimpleGuess,
    BruteForce
};
std::string technique2Str(solver::Technique technique);

enum class LockedCandType
{
    Type1Pointing,
    Type2Claiming
};

enum class HiddenMultiType
{
    Pair = 2,
    Triple = 3,
    Quadruple = 4
};

enum class NakedMultiType
{
    Pair = 2,
    Triple = 3,
    Quadruple = 4
};

enum class BasicFishType
{
    Swordfish = 3,
    Jellyfish = 4
};

} // namespace solver

enum Level
{
    LEV_0_NONE,
    LEV_1_LOGIC,
    LEV_2_LOGIC,
    LEV_3_LOGIC,
    LEV_4_GUESS,
    LEV_5_BRUTE,
    LEV_6_MULTI,
    LEV_UNKNOWN,
    LEV_INVALID
};
std::string level2Str(Level level);

enum class CellAction
{
    AppliedValue,
    RemovedNote,
    InPatternN1,
    InPatternN2,
    ConflictedValue,
    MissingNoteRow,
    MissingNoteCol,
    MissingNoteBlk
};
std::string cellAction2Str(CellAction action);

} // namespace l2sg

#endif // L2SG_ENUMS_H
