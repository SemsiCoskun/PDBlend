#ifndef INTERNALFORCEBONDBASEDBILINEAR_H
#define INTERNALFORCEBONDBASEDBILINEAR_H

#include "calculateWeightedVolume.h"
#include "globalConstants.h"
#include "degradate.h"

#include <vector>
#include <cmath>

void internalForceBondBasedBilinear
(
    const double* xCoord, const double* yCoord, 
    const int* neighborNumbers, const int* neighborList,
    const double* bondConstant, const double* criticalBondStrainBilinear,
    const double* failureBondStrainBilinear, const double* maxTensileBondStrain,
    const int* isNeighbor, const double* dispX, const double* dispY,
    double* currentBondStrain, double* bondHealth, double* fIntX, double* fIntY
);


#endif