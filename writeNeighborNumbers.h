#ifndef WRITENEIGHBORNUMBERS_H
#define WRITENEIGHBORNUMBERS_H

#include <iostream>
#include <fstream>
#include <iomanip> // to use setprecision()
#include <cmath>
#include <vector>

#include "globalConstants.h"
#include <omp.h>

void writeNeighborNumbers
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::string outputPath, const std::string fileName
);

#endif