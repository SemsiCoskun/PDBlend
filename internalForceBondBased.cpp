// References:
// [1] Silling, S., 2000. Reformulation of elasticity theory for discontinuities and long-range forces. Journal of the
//     Mechanics and Physics of Solids 48, 175–209. doi:https://doi.org/10.1016/S0022-5096(99)00029-0.
// [2] Semsi Coskun and Davood Damircheli and Robert Lipton, 2025. Quasi-Brittle Fracture: The Blended Approach. arXiv
//     https://arxiv.org/abs/2503.20109

#include "internalForceBondBased.h"

void internalForceBondBased
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::vector<double>& effectiveHorizonArea,
    const std::vector<double>& dispX, const std::vector<double>& dispY, 
    const std::vector<std::vector<double>>& maxTensileStrain, const std::vector<std::vector<int>>& isNeighbor,
    std::vector<std::vector<double>>& bondHealth, std::vector<double>& fIntX, std::vector<double>& fIntY
)
{
    omp_set_num_threads(nThreads);
    #pragma omp parallel for schedule(static)
    for (size_t nodeI = 0; nodeI < xCoord.size(); ++nodeI) 
    {
        fIntX[nodeI] = 0.0;
        fIntY[nodeI] = 0.0;
        for (size_t j = 0; j < neighborList[nodeI].size(); ++j)
        {
            size_t nodeJ = neighborList[nodeI][j];
            double xi_x = xCoord[nodeJ] - xCoord[nodeI];
            double xi_y = yCoord[nodeJ] - yCoord[nodeI];
            double xi = std::hypot(xi_x, xi_y);

            double vol_i = effectiveHorizonArea[nodeI];
            double vol_j = effectiveHorizonArea[nodeJ];
            double volJ = calculatePartialAreas(xi);

            double eta_x = dispX[nodeJ] - dispX[nodeI];
            double eta_y = dispY[nodeJ] - dispY[nodeI];
            double deformedBond_x = xi_x + eta_x;
            double deformedBond_y = xi_y + eta_y;
            double deformedBond = std::hypot(deformedBond_x, deformedBond_y);
            double extension = deformedBond - xi;
            double bondStrain = extension / xi;
            double maxStrain = maxTensileStrain[nodeI][j];
            int neighFlag = isNeighbor[nodeI][j];

            double scBilinear = std::sqrt(characteristicLength / xi) * ft / (9.0 * E);
            double sfBilinear = (3.0 * M_PI * Gf / (std::sqrt(characteristicLength * xi) * ft));

            // Degradate the material
            bondHealth[nodeI][j] = 
            degradate
            (
                scLinear,
                scBilinear, sfBilinear, 
                bondStrain, maxStrain, neighFlag
            );

            double t = 0.0;
            if (bondStrain < 0.0)
            {
                t = ((2.0 * vol_0 / (vol_i + vol_j)) * bc) * bondStrain; // compression is allowed even if the bond had failed by tension.
            } else
            {
                t = bondHealth[nodeI][j] * ((2.0 * vol_0 / (vol_i + vol_j)) * bc) * bondStrain; // do not carry tension if the bond had already failed by tension
            }

            double dir_x = deformedBond_x / deformedBond;
            double dir_y = deformedBond_y / deformedBond;

            fIntX[nodeI] += t * dir_x * volJ;
            fIntY[nodeI] += t * dir_y * volJ;
        }
    }
    // end of the internal force calculation
}