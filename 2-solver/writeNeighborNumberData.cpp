#include "writeNeighborNumberData.h"

using namespace std;

void writeNeighborNumberData
(
    const std::string outputPath, 
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers
) 
{
    std::ofstream fileNeighborhood(outputPath + "neighborNumbers.csv");
    if (!fileNeighborhood.is_open()) 
    {
        throw runtime_error("Failed to open file for writing neighbor numbers!");
    }
    fileNeighborhood << "1-x-coord" << ',' << "1-y-coord" << ',' << "numOfNeighbor" << '\n';
    for (int i = 0; i < totnode; i++) 
    {
        fileNeighborhood << xCoord[i]*1000 << ',' << yCoord[i]*1000 << ',' << neighborNumbers[i] << "\n";
    }
    fileNeighborhood.close();
}