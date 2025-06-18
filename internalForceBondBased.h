#ifndef INTERNALFORCEBONDBASED_H
#define INTERNALFORCEBONDBASED_H

#include <cmath>
#include <vector>
#include <omp.h>

#include "globalConstants.h"
#include "calculatePartialAreas.h"
#include "degradate.h"

void internalForceBondBased
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::vector<double>& effectiveHorizonArea,
    const std::vector<double>& dispX, const std::vector<double>& dispY, 
    const std::vector<std::vector<double>>& maxTensileStrain, const std::vector<std::vector<int>>& isNeighbor,
    std::vector<std::vector<double>>& bondHealth, std::vector<double>& fIntX, std::vector<double>& fIntY
);

#endif