#ifndef READDATAINTODOUBLEARRAY_H
#define READDATAINTODOUBLEARRAY_H

#include <iostream>
#include <fstream>
#include <string>

void readDataIntoDoubleArray
(
    const std::string& filename, const int& size, double* array
);

#endif