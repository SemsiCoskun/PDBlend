#ifndef CALCULATEDT_H
#define CALCULATEDT_H

#include "globalConstants.h"
#include "calculateWeightedVolume.h"
#include <cmath>

double calculateDt
(
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers, const int* neighborList,
    const double* bondConstant
);

#endif