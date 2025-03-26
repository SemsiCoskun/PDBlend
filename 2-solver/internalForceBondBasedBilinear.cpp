// References:
// [1] Silling, S., 2000. Reformulation of elasticity theory for discontinuities and long-range forces. Journal of the
//  Mechanics and Physics of Solids 48, 175–209. doi:https://doi.org/10.1016/S0022-5096(99)00029-0.

#include "internalForceBondBasedBilinear.h"

void internalForceBondBasedBilinear
(
    const double* xCoord, const double* yCoord, 
    const int* neighborNumbers, const int* neighborList,
    const double* bondConstant, const double* criticalBondStrainBilinear,
    const double* failureBondStrainBilinear, const double* maxTensileBondStrain,
    const int* isNeighbor, const double* dispX, const double* dispY,
    double* currentBondStrain, double* bondHealth, double* fIntX, double* fIntY
)
{
    // Initialize the internal forces
    for (int i = 0; i < totnode; i++)
    {
        fIntX[i] = 0.0;
        fIntY[i] = 0.0;
    }
    double t {};
    int bondIndex {0};
    for (int nodeI = 0; nodeI < totnode; nodeI++) 
    {
        for (int j = 0; j < neighborNumbers[nodeI]; j++) 
        {
            int nodeJ = neighborList[bondIndex];
            double xi_x = xCoord[nodeJ] - xCoord[nodeI];
            double xi_y = yCoord[nodeJ] - yCoord[nodeI];
            double xi = sqrt(xi_x*xi_x + xi_y*xi_y);
            double volJ = calculateWeightedVolume (xi);

            double eta_x = dispX[nodeJ] - dispX[nodeI];
            double eta_y = dispY[nodeJ] - dispY[nodeI];

            double deformedBond_x = xi_x + eta_x;
            double deformedBond_y = xi_y + eta_y;
            double deformedBond = sqrt(deformedBond_x*deformedBond_x + deformedBond_y*deformedBond_y);
            double extension = deformedBond - xi;
            double bondStrain = extension / xi;
            currentBondStrain[bondIndex] = bondStrain;

            double bc = bondConstant[bondIndex];
            degradate
            (
                criticalBondStrainBilinear, failureBondStrainBilinear, 
                maxTensileBondStrain, bondStrain, bondIndex,
                isNeighbor, bondHealth
            );
            
            t = bondHealth[bondIndex] * bc * bondStrain;

            double dir_x = deformedBond_x / deformedBond;
            double dir_y = deformedBond_y / deformedBond;

            fIntX[nodeI] += t * dir_x * volJ;
            fIntY[nodeI] += t * dir_y * volJ;

            bondIndex++;
        }
        //
    }
    // end of the internal force calculation
}