#ifndef CREATENEIGHBORS_H
#define CREATENEIGHBORS_H

#include "geometricalFeatures.h"

void createNeighbors
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const int& totnode, const double& horizon, const double& gridSize,
    const std::vector<Line>& crackLines,
    std::vector<std::vector<int>>& neighbors
);

#endif