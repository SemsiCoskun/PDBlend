#include "calculateDt.h"

using namespace std;

double calculateDt
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::vector<double>& effectiveHorizonArea
)
{
    double dtCritical {0.0};
	double dtCriticalMin {1.0}; // minimum critical dt throughout the domain

    omp_set_num_threads(nThreads);
    #pragma omp parallel for schedule(static)
    for (size_t nodeI = 0; nodeI < xCoord.size(); ++nodeI) 
    {
        double sumTcr = 0.0;

        for (size_t nodeJ : neighborList[nodeI]) 
        {
            double xi_x = xCoord[nodeJ] - xCoord[nodeI];
            double xi_y = yCoord[nodeJ] - yCoord[nodeI];
            double xi = std::hypot(xi_x, xi_y);

            double vol_i = effectiveHorizonArea[nodeI];
            double vol_j = effectiveHorizonArea[nodeJ];
            double volJ = calculatePartialAreas(xi);
            sumTcr += ((2.0 * vol_0 / (vol_i + vol_j)) * bc) / xi * volJ;
        }
        dtCritical = sqrt(2.0 * rho / sumTcr);		
        if (dtCritical < dtCriticalMin) 
        {
            dtCriticalMin = dtCritical;
        }
    }
    
    return (dtSF * dtCriticalMin);
    // end of the time step calculation
}