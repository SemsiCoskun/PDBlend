#ifndef CREATEMESH_H
#define CREATEMESH_H

#include "geometricalFeatures.h"

void createMesh
(
    const double& lengthX, const double& lengthY, 
    const double& gridSize, const double& horizon,
    const std::vector<Rectangle>& exclusionZones,
    std::vector<double>& xCoord, std::vector<double>& yCoord
);

#endif