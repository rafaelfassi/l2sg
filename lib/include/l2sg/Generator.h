#ifndef L2SG_GENERATOR_H
#define L2SG_GENERATOR_H

#include "Grid.h"

namespace l2sg
{

namespace generator
{

void generateRandomFullGrid(Grid &grid);
void generate(Grid &grid, const std::function<int(Grid &)> _solve, bool symmetric = true);
void generateByLevel(Grid &grid, Level targetLevel);
void generateByTechnique(Grid &grid, solver::Technique technique);

} // namespace generator

} // namespace l2sg

#endif // L2SG_GENERATOR_H
