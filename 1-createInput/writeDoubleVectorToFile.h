#ifndef WRITEDOUBLEVECTORTOFILE_H
#define WRITEDOUBLEVECTORTOFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip> 

void writeDoubleVectorToFile
(
    const std::vector<double>& vec, const std::string& filename
) ;

#endif