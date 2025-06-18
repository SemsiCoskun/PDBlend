#ifndef CREATENEIGHBOR_H
#define CREATENEIGHBOR_H

#include <cmath>
#include <vector>
#include <omp.h>

#include "geometricalFeatures.h"
#include "globalConstants.h"
#include "ZonesCrackLines.h"
#include "ZonesExclusion.h"

void createNeighbors
(
    const double& lengthX, const double& lengthY,
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const ZonesCrackLines& cracks,
    std::vector<std::vector<int>>& neighborList
);


#endif