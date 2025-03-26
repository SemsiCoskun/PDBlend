#ifndef WRITEFORCEDISPDATA_H
#define WRITEFORCEDISPDATA_H

#include "globalConstants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // to use setprecision()
#include <cmath>

void writeForceDispData
(
    const std::string outputPath, const int& loadStepNumber, 
    const int& iterationCounter, const int& numBrokenBond,
    const double* xCoord, const double* yCoord,
    const double* dispX, const double* dispY, 
    const int* dispXassignedNodes, const int* dispYassignedNodes,
    const double* fIntX, const double* fIntY
);

#endif