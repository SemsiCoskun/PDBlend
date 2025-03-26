#ifndef CALCULATEDISPLACEMENT_H
#define CALCULATEDISPLACEMENT_H

#include "globalConstants.h"
#include "internalForceBondBasedBilinear.h"
#include "calculateWeightedVolume.h"

void calculateDisplacement
(
    const double* xCoord, const double* yCoord, 
    const int* neighborNumbers, const int* neighborList,
    const int* dispXfixedNodes, const int* dispYfixedNodes,
    const int* dispXassignedNodes, const int* dispYassignedNodes,
    const int* forceXassignedNodes, const int* forceYassignedNodes,
    const double* bondConstant, const double* criticalBondStrainBilinear,
    const double* failureBondStrainBilinear, const double* maxTensileBondStrain, 
    const int* isNeighbor, const double& dtSelected,
    double* fIntX, double* fIntY, double *fExtX, double *fExtY,
    double *accX, double *accY, double *velX, double *velY,
    double *velHalfX, double *velHalfY, double *dispX, double *dispY,
    double* currentBondStrain, double* bondHealth, int &iterationCounter
);


#endif