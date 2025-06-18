#ifndef DEGRADATE_H
#define DEGRADATE_H

#include <cmath>
#include <vector>
#include "globalConstants.h"

double degradate
(
    const double& criticalBondStrainLinear,
    const double& criticalBondStrainBilinear, const double& failureBondStrainBilinear, 
    const double& bondStrain, const double& maxStrain, const int& neighFlag
);

#endif