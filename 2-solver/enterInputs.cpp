
#include "enterInputs.h"

const std::string INPUT_PATH = "../3-IO/input/";

using namespace std;

void enterInputs
(
    const std::string& inputPath, double* xCoord, double* yCoord,
    int* dispXfixedNodes, int* dispYfixedNodes,
    int* dispXassignedNodes, int* dispYassignedNodes,
    int* forceXassignedNodes, int* forceYassignedNodes,
    int* neighborNumbers, int* neighborList
)
{
    std::string filename;

    // Coordinates:
    filename = inputPath + "coordX.txt";
    readDataIntoDoubleArray(filename, totnode, xCoord);
    filename = inputPath + "coordY.txt";
    readDataIntoDoubleArray(filename, totnode, yCoord);

    // Boundary nodes:
    filename = inputPath + "nodesDispXfixed.txt";
    readDataIntoIntArray(filename, numNodeDispXfixed, dispXfixedNodes);
    filename = inputPath + "nodesDispYfixed.txt";
    readDataIntoIntArray(filename, numNodeDispYfixed, dispYfixedNodes);
    filename = inputPath + "nodesDispXassigned.txt";
    readDataIntoIntArray(filename, numNodeDispXassigned, dispXassignedNodes);
    filename = inputPath + "nodesDispYassigned.txt";
    readDataIntoIntArray(filename, numNodeDispYassigned, dispYassignedNodes);
    filename = inputPath + "nodesForceXassigned.txt";
    readDataIntoIntArray(filename, numNodeForceXassigned, forceXassignedNodes);
    filename = inputPath + "nodesForceYassigned.txt";
    readDataIntoIntArray(filename, numNodeForceYassigned, forceYassignedNodes);

    // Neighbors:
    filename = inputPath + "neighborNum.txt";
    readDataIntoIntArray(filename, totnode, neighborNumbers);
    filename = inputPath + "neighborLs.txt";
    readDataIntoIntArray(filename, sizeNeighborList, neighborList);

}