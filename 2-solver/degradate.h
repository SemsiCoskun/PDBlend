#ifndef DEGRADATE_H
#define DEGRADATE_H

void degradate
(
    const double* criticalBondStrainBilinear, const double* failureBondStrainBilinear, 
    const double* maxTensileBondStrain, const double& bondStrain, const int& bondIndex,
    const int* isNeighbor, double* bondHealth
);

#endif