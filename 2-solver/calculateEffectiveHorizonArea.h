#ifndef CALCULATEEFFECTIVEHORIZONAREA_H
#define CALCULATEEFFECTIVEHORIZONAREA_H

#include "globalConstants.h"
#include "calculateWeightedVolume.h"

void calculateEffectiveHorizonArea
(
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers, const int* neighborList,
    double *effectiveHorizonArea
);

#endif