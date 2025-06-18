#ifndef WRITETIMINGINFO_H
#define WRITETIMINGINFO_H

#include <iostream>
#include <fstream>
#include <chrono>  // for high_resolution_clock

void writeTimingInfo
(
    const std::string outputPath, const std::string fileName, const int64_t& duration
);

#endif