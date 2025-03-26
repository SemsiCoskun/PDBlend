#ifndef WRITEDAMAGEDATA_H
#define WRITEDAMAGEDATA_H

#include "globalConstants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // to use setprecision()
#include <cmath>

void writeDamageData
(
    const std::string outputPath, const int& loadStepNumber, 
    const double* xCoord, const double* yCoord,
    const double* dispX, const double* dispY,
    const double* damageLocal
);

#endif