#ifndef L2SG_GENERATOR_H
#define L2SG_GENERATOR_H

#include "Grid.h"

namespace l2sg
{

namespace generator
{

void generateRandomFullGrid(Grid &_grid);
void generate(Grid &_grid, const std::function<int(Grid &)> _solve, bool _symmetric = true);
void generateByLevel(Grid &_grid, Level _targetLevel, bool _symmetric = true);
void generateByTechnique(Grid &_grid, solver::Technique _technique, bool _symmetric = true);

} // namespace generator

} // namespace l2sg

#endif // L2SG_GENERATOR_H
