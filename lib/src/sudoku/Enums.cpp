#include "Enums.h"
#include <stdexcept>

namespace sudoku
{

std::string solver::technique2Str(solver::Technique technique)
{
    switch (technique)
    {
        case Technique::NakedSingles:
            return "NakedSingles";
        case Technique::NakedPair:
            return "NakedPair";
        case Technique::NakedTriple:
            return "NakedTriple";
        case Technique::NakedQuadruple:
            return "NakedQuadruple";
        case Technique::HiddenSingles:
            return "HiddenSingles";
        case Technique::HiddenPair:
            return "HiddenPair";
        case Technique::HiddenTriple:
            return "HiddenTriple";
        case Technique::HiddenQuadruple:
            return "HiddenQuadruple";
        case Technique::LockedCandidatesType1:
            return "LockedCandidatesType1";
        case Technique::LockedCandidatesType2:
            return "LockedCandidatesType2";
        case Technique::XWings:
            return "XWings";
        case Technique::Swordfish:
            return "Swordfish";
        case Technique::Jellyfish:
            return "Jellyfish";
        case Technique::XYWing:
            return "XYWing";
        case Technique::WWing:
            return "WWing";
        case Technique::Skyscraper:
            return "Skyscraper";
        case Technique::SimpleGuess:
            return "SimpleGuess";
        case Technique::BruteForce:
            return "BruteForce";
        default:
            throw std::out_of_range("Technique enum not translated on technique2Str");
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
        default:
            throw std::out_of_range("CellAction enum not translated on cellAction2Str");
    }
}

} // namespace sudoku