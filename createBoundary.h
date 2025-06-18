#ifndef CREATEBOUNDARY_H
#define CREATEBOUNDARY_H

#include <cmath>
#include <vector>

#include "globalConstants.h"
#include "geometricalFeatures.h"
#include "ZonesBoundary.h"

void createBoundary
(
    const double& lengthX, const double& lengthY,
    const std::vector <double>& xCoord, const std::vector <double>& yCoord,
    const ZonesBoundary& boundary,
    std::vector<int>& dispXfixedNodes, std::vector<int>& dispYfixedNodes,
    std::vector<int>& dispXassignedNodes, std::vector<int>& dispYassignedNodes,
    std::vector<int>& velXassignedNodes, std::vector<int>& velYassignedNodes,
    std::vector<int>& forceXassignedNodes, std::vector<int>& forceYassignedNodes,
    std::vector<int>& nodeIdentification, int& cmodNodeA, int& cmodNodeB, int& checkDispNode
);

#endif