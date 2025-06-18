#ifndef CALCULATEDT_H
#define CALCULATEDT_H

#include <cmath>
#include <vector>

#include "globalConstants.h"
#include "calculatePartialAreas.h"
#include <omp.h>

double calculateDt
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::vector<double>& effectiveHorizonArea
);

#endif