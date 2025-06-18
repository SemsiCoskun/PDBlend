#include "calculateEffectiveHorizonArea.h"

#include <iostream>

using namespace std;

void calculateEffectiveHorizonArea
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    std::vector<double>& effectiveHorizonArea
)
{
    omp_set_num_threads(nThreads);
    #pragma omp parallel for schedule(static)
    for (size_t nodeI = 0; nodeI < xCoord.size(); ++nodeI) 
    {
        effectiveHorizonArea[nodeI] = 0.0;
        double sumVol = 0.0;

        for (size_t nodeJ : neighborList[nodeI]) 
        {
            double xi_x = xCoord[nodeJ] - xCoord[nodeI];
            double xi_y = yCoord[nodeJ] - yCoord[nodeI];
            double xi = std::hypot(xi_x, xi_y);

            double volJ = calculatePartialAreas(xi);
            sumVol += volJ;
        }
        effectiveHorizonArea[nodeI] = sumVol + vol;
    }
    // end
}