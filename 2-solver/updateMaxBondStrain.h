#ifndef UPDATEMAXBONDSTRAIN_H
#define UPDATEMAXBONDSTRAIN_H

#include "globalConstants.h"

void updateMaxBondStrain
(
    const double* currentBondStrain, const double* criticalBondStrainBilinear,
    const double* failureBondStrainBilinear, double* maxTensileBondStrain
);

#endif