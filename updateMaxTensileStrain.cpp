#include "updateMaxTensileStrain.h"

void updateMaxTensileStrain
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::vector<double>& dispX, const std::vector<double>& dispY, 
    std::vector<std::vector<double>>& maxTensileStrain
)
{
    omp_set_num_threads(nThreads);
    #pragma omp parallel for schedule(static)
    for (size_t nodeI = 0; nodeI < xCoord.size(); ++nodeI) 
    {
        for (size_t j = 0; j < neighborList[nodeI].size(); ++j)
        {
            size_t nodeJ = neighborList[nodeI][j];
            double xi_x = xCoord[nodeJ] - xCoord[nodeI];
            double xi_y = yCoord[nodeJ] - yCoord[nodeI];
            double xi = std::hypot(xi_x, xi_y);
            double eta_x = dispX[nodeJ] - dispX[nodeI];
            double eta_y = dispY[nodeJ] - dispY[nodeI];
            double deformedBond_x = xi_x + eta_x;
            double deformedBond_y = xi_y + eta_y;
            double deformedBond = std::hypot(deformedBond_x, deformedBond_y);
            double extension = deformedBond - xi;
            double bondStrain = extension / xi;
            double maxStrain = maxTensileStrain[nodeI][j];

            if (bondStrain > maxStrain)
            {
                maxTensileStrain[nodeI][j] = bondStrain;
            }
        }
    }
    // end
}