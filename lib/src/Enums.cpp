#include "Enums.h"
#include <stdexcept>

namespace l2sg
{

std::string solver::technique2Str(solver::Technique technique)
{
    switch (technique)
    {
        case Technique::BadPuzzle:
            return "Bad Puzzle";
        case Technique::NakedSingles:
            return "Naked Single";
        case Technique::NakedPair:
            return "Naked Pair";
        case Technique::NakedTriple:
            return "Naked Triple";
        case Technique::NakedQuadruple:
            return "Naked Quadruple";
        case Technique::HiddenSingles:
            return "Hidden Single";
        case Technique::HiddenPair:
            return "Hidden Pair";
        case Technique::HiddenTriple:
            return "Hidden Triple";
        case Technique::HiddenQuadruple:
            return "Hidden Quadruple";
        case Technique::LockedCandidatesType1:
            return "Pointing";
        case Technique::LockedCandidatesType2:
            return "Claiming";
        case Technique::XWings:
            return "X-Wings";
        case Technique::Swordfish:
            return "Swordfish";
        case Technique::Jellyfish:
            return "Jellyfish";
        case Technique::XYWing:
            return "XY-Wing";
        case Technique::WWing:
            return "W-Wing";
        case Technique::Skyscraper:
            return "Skyscraper";
        case Technique::TwoStringKite:
            return "2-String Kite";
        case Technique::SimpleGuess:
            return "Simple Guess";
        case Technique::BruteForce:
            return "Brute Force";
        default:
            throw std::out_of_range("Technique enum not translated on technique2Str");
    }
}

std::string level2Str(Level level)
{
    switch (level)
    {
    case Level::LEV_0_NONE:
        return "None";
    case Level::LEV_1_LOGIC:
        return "Easy";
    case Level::LEV_2_LOGIC:
        return "Medium";
    case Level::LEV_3_LOGIC:
        return "Hard";
    case Level::LEV_4_GUESS:
        return "Unfair";
    case Level::LEV_5_BRUTE:
        return "Extreme";
    case Level::LEV_6_MULTI:
        return "Multiple Solutions";
    case Level::LEV_UNKNOWN:
        return "Unknow";
    case Level::LEV_INVALID:
        return "Invalid";
    default:
        throw std::out_of_range("Level enum not translated on level2Str");
    }
}

std::string cellAction2Str(CellAction action)
{
    switch (action)
    {
        case CellAction::AppliedValue:
            return "AppliedValue";
        case CellAction::RemovedNote:
            return "RemovedNote";
        case CellAction::InPatternN1:
            return "InPatternN1";
        case CellAction::InPatternN2:
            return "InPatternN2";
        case CellAction::ConflictedValue:
            return "ConflictedValue";
        case CellAction::MissingNoteRow:
            return "MissingNoteRow";
        case CellAction::MissingNoteCol:
            return "MissingNoteCol";
        case CellAction::MissingNoteBlk:
            return "MissingNoteBlk";
        default:
            throw std::out_of_range("CellAction enum not translated on cellAction2Str");
    }
}

} // namespace l2sg