// References:
// [1] ADD ARCHIVE REFERENCE HERE

#include "degradate.h"

void degradate
(
    const double* criticalBondStrainBilinear, const double* failureBondStrainBilinear, 
    const double* maxTensileBondStrain, const double& bondStrain, const int& bondIndex,
    const int* isNeighbor, double* bondHealth
)
{
    double sc = criticalBondStrainBilinear[bondIndex];
    double sf = failureBondStrainBilinear[bondIndex];
    double maxTenStrain = maxTensileBondStrain[bondIndex];

    // Bilinear constitutive model is used.
    if (isNeighbor[bondIndex] == 0) // cannot carry tension if the bond had already failed by tension
    {
        bondHealth[bondIndex] = 0.0;
    } else
    {
        if ((bondStrain <= sc) && (maxTenStrain <= sc)) // linear, no unloading-reloading has occured yet
        {
            bondHealth[bondIndex] = 1.0;
        } else if ((maxTenStrain > sc) && (bondStrain < maxTenStrain)) // softening, unloading-reloading has occured
        {
            bondHealth[bondIndex] = (sc/(sf-sc)) * ((sf/maxTenStrain)-1.0);
        } else if ((bondStrain >= sc) && (bondStrain >= maxTenStrain)) // softening, follow the outer envelope, no unloading
        {
            bondHealth[bondIndex] = (sc/(sf-sc)) * ((sf/bondStrain)-1.0);
        } else if (bondStrain >= sf) // tensile bond strain cannot exceed sf, break the bond
        {
            bondHealth[bondIndex] = 0.0;
        }   
    }
    if (bondStrain < 0.0) // compression
    {
        bondHealth[bondIndex] = 1.0; // compression is allowed even if the bond had failed by tension.
    }
    //
}