#ifndef CALCULATEDISPLACEMENT_H
#define CALCULATEDISPLACEMENT_H

#include <cmath>
#include <vector>
#include <omp.h>

#include "globalConstants.h"
#include "calculatePartialAreas.h"
#include "degradate.h"
#include "internalForceBondBased.h"

void calculateDisplacement
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::vector<double>& effectiveHorizonArea, const double& dtSelected, 
    const std::vector<int> dispXfixedNodes, const std::vector<int> dispYfixedNodes,
    const std::vector<int> dispXassignedNodes, const std::vector<int> dispYassignedNodes,
    const std::vector<int> velXassignedNodes, const std::vector<int> velYassignedNodes,
    const std::vector<int> forceXassignedNodes, const std::vector<int> forceYassignedNodes,
    const std::vector<std::vector<double>>& maxTensileStrain, const std::vector<std::vector<int>>& isNeighbor,
    std::vector<std::vector<double>>& bondHealth, int& iterationCounter,
    std::vector<double>& fIntX, std::vector<double>& fIntY,std::vector<double>& fExtX, std::vector<double>& fExtY,
    std::vector<double>& accX, std::vector<double>& accY, std::vector<double>& velX, std::vector<double>& velY,
    std::vector<double>& velHalfX, std::vector<double>& velHalfY, std::vector<double>& dispX, std::vector<double>& dispY
);


#endif