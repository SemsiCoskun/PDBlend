#include "updateMaxBondStrain.h"

void updateMaxBondStrain
(
    const double* currentBondStrain, const double* criticalBondStrainBilinear,
    const double* failureBondStrainBilinear, double* maxTensileBondStrain
)
{
    for (int i = 0; i < sizeNeighborList; i++)
    {
        double bondStrain = currentBondStrain[i];
        double sc = criticalBondStrainBilinear[i];
        double sf = failureBondStrainBilinear[i];
        double maxTenStrain = maxTensileBondStrain[i];

        if ((maxTenStrain < bondStrain) && (bondStrain > sc))
        {
            maxTensileBondStrain[i] = bondStrain;
        }
        // maximum tensile strain cannot exceed sf
        if (maxTensileBondStrain[i] > sf)
        {
            maxTensileBondStrain[i] = sf;
        }
    }
}