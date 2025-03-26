#ifndef WRITEINTVECTORTOFILE_H
#define WRITEINTVECTORTOFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip> 

void writeIntVectorToFile
(
    const std::vector<int>& vec, const std::string& filename
) ;

#endif