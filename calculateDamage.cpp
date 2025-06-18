#include "calculateDamage.h"

using namespace std;

void calculateDamage
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList, const std::vector<std::vector<double>>& bondHealth,
    std::vector<std::vector<int>>& isNeighbor, std::vector<double>& damageLocal, int& countBrokenNew
)
{
    countBrokenNew = 0;

    omp_set_num_threads(nThreads);
    #pragma omp parallel for schedule(static)
    for (size_t nodeI = 0; nodeI < xCoord.size(); ++nodeI) 
    {
        double neighborArea = 0.0;
        double initialArea = 0.0;
        damageLocal[nodeI] = 0.0;

        for (size_t j = 0; j < neighborList[nodeI].size(); ++j)
        {
            size_t nodeJ = neighborList[nodeI][j];
            double xi_x = xCoord[nodeJ] - xCoord[nodeI];
            double xi_y = yCoord[nodeJ] - yCoord[nodeI];
            double xi = std::hypot(xi_x, xi_y);

            double volJ = calculatePartialAreas(xi);

            if ((isNeighbor[nodeI][j] == 1) && (bondHealth[nodeI][j] < criticalBondHealth))
            {
                isNeighbor[nodeI][j] = 0;
            }
            
            if (isNeighbor[nodeI][j] == 0)
            {
                countBrokenNew++;
            } 

            initialArea += volJ;

            neighborArea += isNeighbor[nodeI][j] * bondHealth[nodeI][j] * volJ;
        }
        damageLocal[nodeI] = 1.0 - neighborArea / initialArea; 
    }
    // end
}