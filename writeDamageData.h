#ifndef WRITEDAMAGEDATA_H
#define WRITEDAMAGEDATA_H

#include "globalConstants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // to use setprecision()
#include <cmath>
#include <vector>

void writeDamageData
(
    const std::string outputPath, const int& loadStepNumber, 
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<double>& dispX, const std::vector<double>& dispY,
    const std::vector<double>& damageLocal
);

#endif