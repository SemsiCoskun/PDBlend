// References:
// [1] ADD ARCHIVE REFERENCE HERE

#include "degradate.h"

double degradate
(
    const double& criticalBondStrainLinear,
    const double& criticalBondStrainBilinear, const double& failureBondStrainBilinear, 
    const double& bondStrain, const double& maxStrain, const int& neighFlag
)
{
    double bondHealth = 1.0;
    
    if (materialModel == 1) // Linear model
    {
        double sc = criticalBondStrainLinear;
        if (neighFlag == 0)
        {
            bondHealth = 0.0;
        } else
        {
            if (bondStrain <= sc)
            {
                bondHealth = 1.0;
            } else
            {
                bondHealth = 0.0;
            }
        }
    } else if (materialModel == 2) // Bilinear model
    {
        double sc = criticalBondStrainBilinear;
        double sf = failureBondStrainBilinear;
        double maxTenStrain = maxStrain;
        if (neighFlag == 0)
        {
            bondHealth = 0.0;
        } else
        {
            // Bilinear constitutive model is used.
            if ((bondStrain <= sc) && (maxTenStrain <= sc)) // linear, no unloading-reloading has occured yet
            {
                bondHealth = 1.0;
            } else if ((maxTenStrain > sc) && (bondStrain < maxTenStrain)) // softening, unloading-reloading has occured
            {
                bondHealth = (sc/(sf-sc)) * ((sf/maxTenStrain)-1.0);
            } else if ((bondStrain >= sc) && (bondStrain >= maxTenStrain)) // softening, follow the outer envelope, no unloading
            {
                bondHealth = (sc/(sf-sc)) * ((sf/bondStrain)-1.0);
            } else if (bondStrain > sf) // tensile bond strain cannot exceed sf, break the bond
            {
                bondHealth = 0.0;
            }   
        }
    } else
    {
        double sc = criticalBondStrainLinear;
        if (neighFlag == 0)
        {
            bondHealth = 0.0;
        } else
        {
            if (bondStrain <= sc)
            {
                bondHealth = 1.0;
            } else
            {
                bondHealth = 0.0;
            }
        }
    }
    //
    return(bondHealth);
    // end of the degradation function
}