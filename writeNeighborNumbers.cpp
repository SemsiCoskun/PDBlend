#include "writeNeighborNumbers.h"

void writeNeighborNumbers
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::string outputPath, const std::string fileName
)
{
    std::vector<int> neighborNumbers(xCoord.size());

    for (size_t nodeI = 0; nodeI < xCoord.size(); ++nodeI) 
    {
        neighborNumbers[nodeI] = neighborList[nodeI].size();
    }
    //
    std::string fullPath = outputPath + fileName;
    std::ofstream file(fullPath);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Failed to open file for writing: " + fullPath);
    }

    file << "x-coord (mm),y-coord (mm),Number of Neighbors\n";
    file << std::fixed << std::setprecision(6);

    for (size_t i = 0; i < xCoord.size(); ++i) 
    {
        file << xCoord[i] * 1e3 << ',' << yCoord[i] * 1e3 << ',' << neighborNumbers[i] << '\n';
    }
    // end
}
