#ifndef CALCULATEDAMAGE_H
#define CALCULATEDAMAGE_H

#include <cmath>
#include <vector>

#include "globalConstants.h"
#include "calculatePartialAreas.h"
#include <omp.h>

void calculateDamage
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList, const std::vector<std::vector<double>>& bondHealth,
    std::vector<std::vector<int>>& isNeighbor, std::vector<double>& damageLocal, int& countBrokenNew
);

#endif