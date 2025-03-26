#ifndef CALCULATEDAMAGE_H
#define CALCULATEDAMAGE_H

#include "globalConstants.h"
#include "calculateWeightedVolume.h"
#include <cmath>


void calculateDamage
(
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers, const int* neighborList,
    const double* bondHealth, int* isNeighbor,
    double* damageLocal, int &countBrokenNew
);

#endif