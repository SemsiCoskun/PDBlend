#include "writeForceDispData.h"

using namespace std;

void writeForceDispData
(
    const std::string outputPath, const int& loadStepNumber, 
    const int& iterationCounter, const int& numBrokenBond,
    const double* xCoord, const double* yCoord,
    const double* dispX, const double* dispY, 
    const int* dispXassignedNodes, const int* dispYassignedNodes,
    const double* fIntX, const double* fIntY
)
{
    double sumForcesIntX = 0.0; 
    double sumForcesIntY = 0.0;
    
    // Calculate sum of internal forces
    for (int i = 0; i < numNodeDispYassigned; i++) {
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