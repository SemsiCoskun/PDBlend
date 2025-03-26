#ifndef ENTERINPUTS_H
#define ENTERINPUTS_H

#include "globalConstants.h"

#include "readDataIntoDoubleArray.h"
#include "readDataIntoIntArray.h"

void enterInputs
(
    const std::string& inputPath, double* xCoord, double* yCoord,
    int* dispXfixedNodes, int* dispYfixedNodes,
    int* dispXassignedNodes, int* dispYassignedNodes,
    int* forceXassignedNodes, int* forceYassignedNodes,
    int* neighborNumbers, int* neighborList
);


#endif