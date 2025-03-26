#ifndef WRITENODEIDENTIFICATIONDATA_H
#define WRITENODEIDENTIFICATIONDATA_H

#include "globalConstants.h"
#include <iostream>
#include <fstream>

void writeNodeIdentificationData
(
    const std::string& outputPath, const double* xCoord, const double* yCoord,
    const int* dispXfixedNodes, const int* dispYfixedNodes,
    const int* dispXassignedNodes, const int* dispYassignedNodes,
    const int* forceXassignedNodes, const int* forceYassignedNodes
);

#endif