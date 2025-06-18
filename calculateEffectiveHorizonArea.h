#ifndef CALCULATEEFFECTIVEHORIZONAREA_H
#define CALCULATEEFFECTIVEHORIZONAREA_H

#include <cmath>
#include <vector>

#include "globalConstants.h"
#include "calculatePartialAreas.h"
#include <omp.h>

void calculateEffectiveHorizonArea
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    std::vector<double>& effectiveHorizonArea
);

#endif