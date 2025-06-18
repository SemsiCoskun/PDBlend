#ifndef WRITEFORCEDISPDATA_H
#define WRITEFORCEDISPDATA_H

#include "globalConstants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // to use setprecision()
#include <cmath>
#include <vector>

void writeForceDispData
(
    const std::string outputPath, const int& loadStepNumber, 
    const int& iterationCounter, const int& numBrokenBond,
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<double>& dispX, const std::vector<double>& dispY,
    const std::vector<int>& dispXassignedNodes, const std::vector<int>& dispYassignedNodes,
    const std::vector<double>& fIntX, const std::vector<double>& fIntY,
    const int& cmodNodeA, const int& cmodNodeB, const int& checkDispNode
);

#endif