#include "Logs.h"
#include <algorithm>

namespace l2sg
{

void solver::Log::dump(std::ostream &out)
{
    out << solver::technique2Str(technique) << std::endl;

    std::sort(cellLogs.begin(), cellLogs.end(),
              [](const CellLog &a, const CellLog &b)
              {
                  if (a.action != b.action)
                      return a.action > b.action;
                  else if (a.cell.first != b.cell.first)
                      return a.cell.first < b.cell.first;
                  else if (a.cell.second != b.cell.second)
                      return a.cell.second < b.cell.second;
                  else
                      return a.value < b.value;
              });
    for (const auto &cellLog : cellLogs)
    {
        out << "\t" << cellAction2Str(cellLog.action) //
            << " V: " << cellLog.value                //
            << " At: " << cellLog.cell.first << "," << cellLog.cell.second << std::endl;
    }
}

} // namespace l2sg