// References:
// Material constants are borrowed from:
// [1] Chen, H., Liu, D., 2023. Fracture process zone of high-strength concrete under monotonic and cyclic loading.
//      Engineering Fracture Mechanics 277, 108973. doi:https://doi.org/10.1016/j.engfracmech.2022.108973.

#include <iostream>
#include <fstream>
#include <iomanip> // to use setprecision()
#include <chrono>  // for high_resolution_clock
// To make directories
#include <sys/stat.h>
#include <sys/types.h>

#include <string>

#include "geometricalFeatures.h"
#include "createMesh.h"
#include "createNeighbors.h"
#include "writeDoubleVectorToFile.h"
#include "writeIntVectorToFile.h"


using namespace std;

const std::string INPUT_PATH = "../3-IO/input/";

int main ()
{
    // Record start time
	auto start = chrono::high_resolution_clock::now();

    std::cout << "\nLet's begin to create input files!\n";

    // Material Constants
    double fc = 81.83e+6;  // Ultimate Compressive Strength (Pa)
    double E = 33.89e+9;   // Elastic modulus (Pa)
    double rho = 2500.0;   // Density (kg/m^3)
    double nu = 1.0 / 3.0; // Poisson ratio
    double ft = 3.39e+6;   // Ultimate Tensile Strength (Pa)
    double Gf = 116.15;    // Fracture Energy (J/ m^2)

    double localDamping = 2.0e+7;  // local damping coefficient kg/(m^3 s)
    double tolerance = 1.0e-5;     // relative abs percent error tolerance
    int maxIterationNumber = 10e3; // maximum number of iterations allowed to reach static equilibrium at a load step
    int totalNumLoadStep = 200;    // total number of load steps

    double finalDisplacement = 0.80e-3; // the target displacement
    double numCycles = 0; // zero if monothonic, enter the number of cyles if the applied load is cyclic
    double dtSF = 0.85;   // safety factor used in the critical time step calculation

    // For the original greatest clear domain (not including any zones/domains)
    double lengthX = 708.0e-3;  // width of the domain (m)
    double lengthY = 150.0e-3;  // height of the domain (m)
    double thickness = 50.0e-3; // thickness of the domain (m)
    double gridSize = 2.0e-3;   // uniform grid size (m)
    double horizon = gridSize * 3.015;
    
    double area = gridSize * thickness;   // uniform cross-sectional area of a material point
    double vol = area * gridSize;         // uniform volume of a material point
    double boundaryLayer = gridSize*4.01; // length of the boundary regions

    double beamDepth = lengthY;           // beam depth (m)
    double span = 600.0e-3;               // span length (m)
    double crackLength = 45.0e-3;         // crack length (m)
    double crackThickness = 0.8*gridSize; // used for a narrow rectangular region to define crack
    double supportOffset = 0.5*(lengthX - span); // horizontal distance between the support and the origin
    double crackOffset = 0.0;                    // zero corresponds to mode-I failure

    // DEFINE ZONES
    lengthX = lengthX + 2.0*boundaryLayer;
    lengthY = lengthY + 2.0*boundaryLayer;
    // Exclusion Zones
    double bottomLeftX = -0.5*lengthX; double bottomLeftY = -0.5*lengthY;
    Rectangle subtractZone1({bottomLeftX, bottomLeftY}, supportOffset + 0.5*boundaryLayer, boundaryLayer);

    bottomLeftX = -0.5*(span - boundaryLayer); bottomLeftY = -0.5*lengthY;
    Rectangle subtractZone2({bottomLeftX, bottomLeftY}, span - boundaryLayer, boundaryLayer);

    bottomLeftX = 0.5*span + 0.5*boundaryLayer; bottomLeftY = -0.5*lengthY;
    Rectangle subtractZone3({bottomLeftX, bottomLeftY}, supportOffset + 0.5*boundaryLayer, boundaryLayer);

    bottomLeftX = 0.5*span + supportOffset; bottomLeftY = -0.5*beamDepth;
    Rectangle subtractZone4({bottomLeftX, bottomLeftY}, boundaryLayer, beamDepth);

    bottomLeftX = 0.5*boundaryLayer; bottomLeftY = 0.5*beamDepth;
    Rectangle subtractZone5({bottomLeftX, bottomLeftY}, 0.5*span + supportOffset + 0.5*boundaryLayer, boundaryLayer);

    bottomLeftX = -0.5*lengthX; bottomLeftY = 0.5*beamDepth;
    Rectangle subtractZone6({bottomLeftX, bottomLeftY}, 0.5*span + supportOffset + 0.5*boundaryLayer, boundaryLayer);

    bottomLeftX = -0.5*lengthX;  bottomLeftY = -0.5*beamDepth;
    Rectangle subtractZone7({bottomLeftX, bottomLeftY}, boundaryLayer, beamDepth);

    // Define the edges of the pre-crack zones as the pre-crack lines
    Rectangle preCrack1 = {{crackOffset-0.5*crackThickness, -0.5*beamDepth}, crackThickness, crackLength + 0.45*gridSize};

    // Combine the exclusion zones into one set
    std::vector<Rectangle> exclusionZones = 
    {
        subtractZone1, subtractZone2, subtractZone3, subtractZone4, 
        subtractZone5, subtractZone6, subtractZone7, preCrack1
    };

    // Define the edges of the pre-crack zones as the pre-crack lines
    std::vector<Line> crackLines = 
    {
        preCrack1.l2, preCrack1.l4, preCrack1.l1, preCrack1.l3
    };

    // CREATE MESH 
    // x and y coordinates of the material points
    std::vector<double> xCoord;
    std::vector<double> yCoord;
    createMesh
    (
        lengthX, lengthY, 
        gridSize, horizon,
        exclusionZones,
        xCoord, yCoord
    );
    int totnode = xCoord.size();
    
    int nodeIdentification[totnode];
    for (int i = 0; i < totnode; i++)
    {
        nodeIdentification[i] = 0;
    }

    // CREATE BOUNDARY ZONES
    // support
    std::vector<int> dispXfixedNodes;
    std::vector<int> dispYfixedNodes;
    bottomLeftX = -0.5*span - 0.5*boundaryLayer; bottomLeftY = -0.5*lengthY;
    Rectangle boundaryZone({bottomLeftX, bottomLeftY}, boundaryLayer, boundaryLayer);
    for (int i = 0; i < totnode; i++)
    {
        Point node = {xCoord[i], yCoord[i]};
        // Include the nodes in the list
        if (isInsideZone(node, boundaryZone)) 
        {
            dispXfixedNodes.push_back(i);
            dispYfixedNodes.push_back(i);
            nodeIdentification[i] = 1;
        }
    }

    bottomLeftX = 0.5*span - 0.5*boundaryLayer; bottomLeftY = -0.5*lengthY;
    boundaryZone = Rectangle({bottomLeftX, bottomLeftY}, boundaryLayer, boundaryLayer);
    for (int i = 0; i < totnode; i++)
    {
        Point node = {xCoord[i], yCoord[i]};
        // Include the nodes in the list
        if (isInsideZone(node, boundaryZone)) 
        {
            dispYfixedNodes.push_back(i);
            nodeIdentification[i] = 2;
        }
    }
    
    // loading
    std::vector<int> dispXassignedNodes;
    std::vector<int> dispYassignedNodes;
    bottomLeftX = -0.5*boundaryLayer; bottomLeftY = 0.5*beamDepth;
    boundaryZone = Rectangle({bottomLeftX, bottomLeftY}, boundaryLayer, boundaryLayer);
    for (int i = 0; i < totnode; i++)
    {
        Point node = {xCoord[i], yCoord[i]};
        // Include the nodes in the list
        if (isInsideZone(node, boundaryZone)) 
        {
            dispYassignedNodes.push_back(i);
            nodeIdentification[i] = 3;
        }
    }
    std::vector<int> forceYassignedNodes;
    std::vector<int> forceXassignedNodes;
    // Nodes to measure crack mount opening distance
    int cmodNodeA; int cmodNodeB;
    for (int i = 0; i < totnode; i++)
    {
        if ((fabs(xCoord[i] - preCrack1.bottomLeft.x) <= 0.85*gridSize) && (fabs(yCoord[i] - (preCrack1.bottomLeft.y)) <= 0.85*gridSize) && (xCoord[i] < preCrack1.bottomLeft.x))
        {
            cmodNodeA = i;
            nodeIdentification[i] = 4;
        } 
        
        if ((fabs(xCoord[i] - preCrack1.bottomRight.x) <= 0.85*gridSize) && (fabs(yCoord[i] - (preCrack1.bottomRight.y)) <= 0.85*gridSize) && (xCoord[i] > preCrack1.bottomRight.x))
        {
            cmodNodeB = i;
            nodeIdentification[i] = 5;
        }        
    }
    // Node to measure the reaction for force-disp output data
    int checkDispNode = dispYassignedNodes[floor(0.5*dispYassignedNodes.size())];
    nodeIdentification[checkDispNode] = 6;
    // Write the node identification numbers
    ofstream fileNodeIdentification (INPUT_PATH + "/nodeIdentificationPlot.csv");
    fileNodeIdentification << "1-x-coord (mm)" << ',' << "2-y-coord  (mm)" << ',' << "Node Identification" << '\n';
    for (int i = 0; i <  totnode; i++)
    {
        fileNodeIdentification << xCoord[i]*1.0e+3 << ',' << yCoord[i]*1.0e+3 << ',' << nodeIdentification[i] << '\n';
    }
    fileNodeIdentification.close();

    // Write Coordinates
    writeDoubleVectorToFile(xCoord, INPUT_PATH + "coordX.txt");
    writeDoubleVectorToFile(yCoord, INPUT_PATH + "coordY.txt");
    // Boundary Nodes
    writeIntVectorToFile(dispXfixedNodes, INPUT_PATH + "nodesDispXfixed.txt");
    writeIntVectorToFile(dispYfixedNodes, INPUT_PATH + "nodesDispYfixed.txt");
    writeIntVectorToFile(dispXassignedNodes, INPUT_PATH + "nodesDispXassigned.txt");
    writeIntVectorToFile(dispYassignedNodes,  INPUT_PATH + "nodesDispYassigned.txt");
    writeIntVectorToFile(forceXassignedNodes, INPUT_PATH + "nodesForceXassigned.txt");
    writeIntVectorToFile(forceYassignedNodes, INPUT_PATH + "nodesForceYassigned.txt");

    // Create Neighborhood
    std::vector<std::vector<int>> neighbors(totnode);
    createNeighbors
    (
        xCoord, yCoord,
        totnode, horizon, gridSize,
        crackLines,
        neighbors
    );
    int neighborNumbers[totnode];
    int sizeNeighborList = 0;
    for (int i = 0; i < totnode; i++)
    {
        neighborNumbers[i] = neighbors[i].size();
        sizeNeighborList += neighborNumbers[i];
    }
    // Open the file in write mode
    std::ofstream fileNeighNum(INPUT_PATH + "neighborNum.txt");
    for (int i = 0; i < totnode; i++)
    {
        fileNeighNum << neighborNumbers[i] << "\n";
    }
    fileNeighNum.close();
    std::ofstream fileNeighLs(INPUT_PATH + "neighborLs.txt");
    for (int i = 0; i < totnode; i++)
    {
        for (size_t j = 0; j < neighbors[i].size(); j++)
        {
            fileNeighLs << neighbors[i][j] << "\n";
        }
    }
    fileNeighLs.close();

    ofstream fileNodeNeighNumPlot (INPUT_PATH + "/nodeNeighNumPlot.csv");
    fileNodeNeighNumPlot << "1-x-coord (mm)" << ',' << "2-y-coord  (mm)" << ',' << "Number of Neighbors" << '\n';
    for (int i = 0; i <  totnode; i++)
    {
        fileNodeNeighNumPlot << xCoord[i]*1.0e+3 << ',' << yCoord[i]*1.0e+3 << ',' << neighborNumbers[i] << '\n';
    }
    fileNodeNeighNumPlot.close();
    
    // CREATE globalConstants.h FILE
    // Open the file in write mode
    std::ofstream file(INPUT_PATH + "globalConstants.h");
    // Check if the file opened successfully
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file!" << std::endl;
    }
    // Write Inputs
    file << "#ifndef GLOBALCONSTANTS_H" << "\n";
    file << "#define GLOBALCONSTANTS_H" << "\n";

    file << "\n";

    file << "const int totnode = " << totnode << ";\n";
    file << "const int sizeNeighborList = " << sizeNeighborList << ";\n";

    file << "const int cmodNodeA = " << cmodNodeA << ";\n";
    file << "const int cmodNodeB = " << cmodNodeB << ";\n";
    file << "const int checkDispNode = " << checkDispNode << ";\n";

    file << "const int numNodeDispXfixed = " << dispXfixedNodes.size() << ";\n";
    file << "const int numNodeDispYfixed = " << dispYfixedNodes.size() << ";\n";
    file << "const int numNodeDispXassigned = " << dispXassignedNodes.size() << ";\n";
    file << "const int numNodeDispYassigned = " << dispYassignedNodes.size() << ";\n";
    file << "const int numNodeForceXassigned = " << forceXassignedNodes.size() << ";\n";
    file << "const int numNodeForceYassigned = " << forceYassignedNodes.size() << ";\n";

    file << "const double fc = " << fc << ";\n";
    file << "const double E = " << E << ";\n";
    file << "const double ft = " << ft << ";\n";
    file << "const double Gf = " << Gf << ";\n";
    file << "const double rho = " << rho<< ";\n";
    file << "const double nu = " << nu << ";\n";

    file << "const double localDamping = " << localDamping << ";\n";
    file << "const double tolerance = " << tolerance << ";\n";
    file << "const int maxIterationNumber = " << maxIterationNumber << ";\n";
    file << "const int totalNumLoadStep = " << totalNumLoadStep << ";\n";

    file << "const double finalDisplacement = " << finalDisplacement << ";\n";
    file << "const double numCycles = " << numCycles << ";\n";
    file << "const double dtSF = " << dtSF << ";\n";

    file << "const double characteristicLength = " << beamDepth << ";\n";
    file << "const double gridSize = " << gridSize << ";\n";
    file << "const double horizon = " << horizon << ";\n";
    file << "const double thickness = " << thickness << ";\n";
    file << "const double vol = " << vol << ";\n";

    file << "#endif"; 
    file.close();

    std::cout << "Input files were written successfully.\n";
       
    // Record end time
	auto finish = chrono::high_resolution_clock::now();
	auto duration   = chrono::duration_cast<chrono::milliseconds>( finish - start ).count();
    cout << endl;
	cout << "time elapsed is " << duration << " milliseconds = ";
    cout << (int) ((duration / (1000*60*60)) % 24) << " hours, " <<  (int) ((duration / (1000*60)) % 60) << " minutes, and " << (int) (duration / 1000) % 60 << " seconds.\n";
    // Write the run-time into text file
    ofstream fileEnd (INPUT_PATH + "runEnd-createInput.txt");
    fileEnd << "time elapsed is " << duration << " milliseconds = ";
    fileEnd << (int) ((duration / (1000*60*60)) % 24) << " hours, " <<  (int) ((duration / (1000*60)) % 60) << " minutes, and " << (int) (duration / 1000) % 60 << " seconds.\n";    
    fileEnd.close();
    
    //
	return 0;
}