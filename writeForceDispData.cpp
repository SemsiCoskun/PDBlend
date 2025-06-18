#include "writeForceDispData.h"

using namespace std;

void writeForceDispData
(
    const std::string outputPath, const int& loadStepNumber, 
    const int& iterationCounter, const int& numBrokenBond,
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<double>& dispX, const std::vector<double>& dispY,
    const std::vector<int>& dispXassignedNodes, const std::vector<int>& dispYassignedNodes,
    const std::vector<double>& fIntX, const std::vector<double>& fIntY,
    const int& cmodNodeA, const int& cmodNodeB, const int& checkDispNode
)
{
    double sumForcesIntX = 0.0; 
    double sumForcesIntY = 0.0;
    
    // Calculate sum of internal forces
    for (size_t i = 0; i < dispYassignedNodes.size(); i++) {
        int nodeNumber = dispYassignedNodes[i]; 
        sumForcesIntX += fIntX[nodeNumber] * vol;
        sumForcesIntY += fIntY[nodeNumber] * vol;
    }

    // Calculate CMOD
    double pointAnewPosition = xCoord[cmodNodeA] + dispX[cmodNodeA];
    double pointBnewPosition = xCoord[cmodNodeB] + dispX[cmodNodeB];
    double xDistanceAB = fabs(xCoord[cmodNodeA] - xCoord[cmodNodeB]);
    
    // Write to file
    std::ofstream fileForceDisp(outputPath + "force-disp.csv", std::ios_base::app);
    if (!fileForceDisp.is_open()) 
    {
        throw runtime_error("Failed to open force-displacement file for appending");
    }
    
    fileForceDisp << setprecision(6) 
                 << loadStepNumber << ',' << iterationCounter << ',' << numBrokenBond << ',' 
                 << (dispX[checkDispNode])*1.0e3 << ',' << (sumForcesIntX)*1.0e-3 << ',' 
                 << (dispY[checkDispNode])*1.0e3 << ',' 
                 << (pointBnewPosition - pointAnewPosition - xDistanceAB)*1.0e3 << ',' 
                 << (sumForcesIntY)*1.0e-3 << '\n';
    fileForceDisp.close();
}