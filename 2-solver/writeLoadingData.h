#ifndef WRITELOADINGDATA_H
#define WRITELOADINGDATA_H

#include "globalConstants.h"
#include <iostream>
#include <fstream>
#include <string>

void writeLoadingData
(
    const std::string outputPath, const double* appliedLoading
);

#endif