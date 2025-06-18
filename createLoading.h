#ifndef CREATELOADING_H
#define CREATELOADING_H

#include <cmath>
#include <vector>

#include "globalConstants.h"

void createLoading
(
    const double& finalLoad, const int& totalLoadingStep,
    std::vector<double>& appliedLoading
);

#endif