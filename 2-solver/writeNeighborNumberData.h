#ifndef WRITENEIGHBORNUMBERDATA_H
#define WRITENEIGHBORNUMBERDATA_H

#include "globalConstants.h"
#include <iostream>
#include <fstream>
#include <string>

void writeNeighborNumberData
(
    const std::string outputPath, 
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers
);

#endif