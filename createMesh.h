#ifndef CREATEMESH_H
#define CREATEMESH_H

#include <cmath>
#include <vector>

#include "globalConstants.h"
#include "geometricalFeatures.h"
#include "ZonesExclusion.h"

void createMesh
(
    const double& lengthX, const double& lengthY,
    const ZonesExclusion& exclusion,
    std::vector <double>& xCoord, std::vector <double>& yCoord
);

#endif