#ifndef UPDATEMAXTENSILESTRAIN_H
#define UPDATEMAXTENSILESTRAIN_H

#include <cmath>
#include <vector>
#include <omp.h>

#include "globalConstants.h"

void updateMaxTensileStrain
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::vector<double>& dispX, const std::vector<double>& dispY, 
    std::vector<std::vector<double>>& maxTensileStrain
);

#endif