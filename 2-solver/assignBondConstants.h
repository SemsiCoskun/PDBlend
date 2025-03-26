#ifndef ASSIGNBONDCONSTANTS_H
#define ASSIGNBONDCONSTANTS_H

#include "globalConstants.h"
#include "calculateWeightedVolume.h"
#include <cmath>


void assignBondConstants
(
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers, const int* neighborList,
    const double* effectiveHorizonArea,
    const double* youngsModulus, const double* tensileStrength, const double* fractureEnergy,
    double* bondConstant, double* criticalBondStrainLinear,
    double* criticalBondStrainBilinear, double* failureBondStrainBilinear
);

#endif