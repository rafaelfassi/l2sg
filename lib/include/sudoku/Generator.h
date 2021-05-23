#ifndef SUDOKU_GENERATOR_H
#define SUDOKU_GENERATOR_H

#include "Grid.h"

namespace sudoku
{

namespace generator
{

void generate(Grid &grid, const std::function<int(Grid &)> _solve, bool symmetric = true);
void generateByLevel(Grid &grid, Level targetLevel);
void generateByTechnique(Grid &grid, solver::Technique technique);

} // namespace generator

} // namespace sudoku

#endif // SUDOKU_GENERATOR_H
