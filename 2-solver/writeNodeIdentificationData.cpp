#include "writeNodeIdentificationData.h"
#include <string>

using namespace std;

void writeNodeIdentificationData
(
    const std::string& outputPath, const double* xCoord, const double* yCoord,
    const int* dispXfixedNodes, const int* dispYfixedNodes,
    const int* dispXassignedNodes, const int* dispYassignedNodes,
    const int* forceXassignedNodes, const int* forceYassignedNodes
)
{
    // This function is used for checking the node identification data 
    // and see whether boundary conditions are applied as you wish
    int nodeIdentification[totnode];
    for (int i = 0; i < totnode; i++)
    {
        nodeIdentification[i] = 0;
    }
    for (int i = 0; i < numNodeDispXfixed; i++)
    {
        int nodeNumber = dispXfixedNodes[i];
        nodeIdentification[nodeNumber] = 1;
    }
    for (int i = 0; i < numNodeDispYfixed; i++)
    {
        int nodeNumber = dispYfixedNodes[i];
        nodeIdentification[nodeNumber] = 2;
    }
    for (int i = 0; i < numNodeDispXassigned; i++)
    {
        int nodeNumber = dispXassignedNodes[i];
        nodeIdentification[nodeNumber] = 3;
    }
    for (int i = 0; i < numNodeDispYassigned; i++)
    {
        int nodeNumber = dispYassignedNodes[i];
        nodeIdentification[nodeNumber] = 4;
    }
    for (int i = 0; i < numNodeForceXassigned; i++)
    {
        int nodeNumber = forceXassignedNodes[i];
        nodeIdentification[nodeNumber] = 5;
    }
    for (int i = 0; i < numNodeForceYassigned; i++)
    {
        int nodeNumber = forceYassignedNodes[i];
        nodeIdentification[nodeNumber] = 6;
    }
    if (cmodNodeA != 0) nodeIdentification[cmodNodeA] = 7;
    if (cmodNodeB != 0) nodeIdentification[cmodNodeB] = 8;
    if (checkDispNode != 0) nodeIdentification[checkDispNode] = 9;

    std::ofstream fileNodeIdentification (outputPath + "nodeIdentification.csv");
    fileNodeIdentification << "1-x-coord (mm)" << ',' << "2-y-coord  (mm)" << ',' << "Node Identification" << '\n';
    for (int i = 0; i <  totnode; i++)
    {
        fileNodeIdentification << xCoord[i]*1.0e+3 << ',' << yCoord[i]*1.0e+3 << ',' << nodeIdentification[i] << '\n';
    }
    fileNodeIdentification.close();
}