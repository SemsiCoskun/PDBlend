#include <iostream>
#include <fstream>
#include <iomanip> // to use setprecision()
#include <cmath>
#include <vector>

#include <chrono>  // for high_resolution_clock
// To make directories
#include <sys/stat.h>
#include <sys/types.h>


#include "globalConstants.h"

#include "createMesh.h"
#include "ZonesExclusion.h"
#include "ZonesBoundary.h"
#include "ZonesCrackLines.h"

#include "createBoundary.h"
#include "createNeighbors.h"
#include "calculateEffectiveHorizonArea.h"
#include "calculateDt.h"


#include "internalForceBondBased.h"

#include "calculateDamage.h"
#include "createLoading.h"
#include "calculateDisplacement.h"
#include "updateMaxTensileStrain.h"

#include "writeNeighborNumbers.h"
#include "writeForceDispData.h"
#include "writeDamageData.h"
#include "writeTimingInfo.h"


using namespace std;

void initializeForceDispFile
(
    const std::string outputPath
) 
{
    std::ofstream fileForceDisp(outputPath + "force-disp.csv");
    if (!fileForceDisp.is_open()) 
    {
        throw runtime_error("Failed to open force-displacement file for writing");
    }
    
    fileForceDisp << "loadStepNumber" << ',' << "numIteration" << ',' << "NumberOfBrokenBond" << ',' 
                 << "dispX (mm)" << ',' << "forceX (kN)" << ',' 
                 << "dispY (mm)" << ',' << "CMOD (mm)" << ',' 
                 << "forceY (kN)" << '\n';
    fileForceDisp.close();
}


int main ()
{
    // Record start time
	auto start = chrono::high_resolution_clock::now();

    // Create a folder to store the output data
    std::string outputPath = "outputData/";
    mkdir(outputPath.c_str(), 0777);
    mkdir("outputData/damageData", 0777);
    
    // Greater Domain and Zones
    double lengthX = domainX;
    double lengthY = domainY + 2.0*boundaryLayer;

    ZonesExclusion exclusion(lengthX, lengthY);
    ZonesBoundary boundary(lengthX, lengthY);
    ZonesCrackLines cracks(lengthX, lengthY, exclusion, boundary);

    // Create Mesh
    std::vector<double> xCoord;
    std::vector<double> yCoord;
    createMesh
    (
        lengthX, lengthY,
        exclusion,
        xCoord, yCoord
    );
    int totnode = xCoord.size();

    // Create boundary node vectors
    std::vector<int> dispXfixedNodes; std::vector<int> dispYfixedNodes;
    std::vector<int> dispXassignedNodes; std::vector<int> dispYassignedNodes;
    std::vector<int> velXassignedNodes; std::vector<int> velYassignedNodes;
    std::vector<int> forceXassignedNodes; std::vector<int> forceYassignedNodes;

    // Nodes to measure crack mount opening distance
    int cmodNodeA; int cmodNodeB;
    // Node to measure the reaction for force-disp output data
    int checkDispNode;
    std::vector<int> nodeIdentification(totnode);
    createBoundary
    (
        lengthX, lengthY,
        xCoord, yCoord,
        boundary,
        dispXfixedNodes, dispYfixedNodes,
        dispXassignedNodes, dispYassignedNodes,
        velXassignedNodes, velYassignedNodes,
        forceXassignedNodes, forceYassignedNodes,
        nodeIdentification, cmodNodeA, cmodNodeB, checkDispNode
    );
    std::ofstream fileNodeIdentification (outputPath + "nodeIdentification.csv");
    fileNodeIdentification << "x-coord (mm),y-coord (mm),Node Identification" << '\n';
    for (int i = 0; i <  totnode; i++)
    {
        fileNodeIdentification << xCoord[i]*1e3 << ',' << yCoord[i]*1e3 << ',' << nodeIdentification[i] << '\n';
    }
    fileNodeIdentification.close();

    // Create Neighbor List
    std::vector<std::vector<int>> neighborList(totnode);
    createNeighbors
    (
        lengthX, lengthY,
        xCoord, yCoord,
        cracks,
        neighborList
    );
    // Write the number of neighbors for each node
    std::string fileName = "numberOfNeighbors.csv";
    writeNeighborNumbers
    (
        xCoord,yCoord,
        neighborList, 
        outputPath, fileName
    );

    // Calculate effective horizon areas to apply surface corrections
    std::vector<double> effectiveHorizonArea(totnode);
    calculateEffectiveHorizonArea
    (
        xCoord, yCoord,
        neighborList,
        effectiveHorizonArea
    );

    // Calculate time step
    double dtSelected = 
    calculateDt
    (
        xCoord, yCoord,
        neighborList,
        effectiveHorizonArea
    );
    std::cout << std::endl;
    std::cout << setprecision(6) << "dtSelected = " << dtSelected << " seconds." << std::endl;
    std::cout << std::endl;

    // Damage Related Vectors
    std::vector<double> damageLocal(totnode);
    std::ofstream fileInitialDamage (outputPath + "initialDamage.csv");
    fileInitialDamage << "x-coord (mm),y-coord (mm),Local Damage" << '\n';
    for (int i = 0; i <  totnode; i++)
    {
        fileInitialDamage << xCoord[i]*1e3 << ',' << yCoord[i]*1e3 << ',' << damageLocal[i] << '\n';
    }
    fileInitialDamage.close();

    std::vector<std::vector<double>> bondHealth(totnode);
    for (int i = 0; i < totnode; ++i) 
    {
        bondHealth[i].resize(neighborList[i].size(), 1.0);
    }
    std::vector<std::vector<int>> isNeighbor(totnode);
    for (int i = 0; i < totnode; ++i) 
    {
        isNeighbor[i].resize(neighborList[i].size(), 1);
    }
    std::vector<std::vector<double>> maxTensileStrain(totnode);
    if (materialModel == 1)
    {
        for (int i = 0; i < totnode; ++i) 
        {
            maxTensileStrain[i].resize(neighborList[i].size(), scLinear);
        }
    } else
    {
        for (int i = 0; i < totnode; ++i) 
        {
            maxTensileStrain[i].resize(neighborList[i].size(), 1.0);
        }
    }
    for (size_t nodeI = 0; nodeI < xCoord.size(); ++nodeI) 
    {
        for (size_t j = 0; j < neighborList[nodeI].size(); ++j)
        {
            size_t nodeJ = neighborList[nodeI][j];
            double xi_x = xCoord[nodeJ] - xCoord[nodeI];
            double xi_y = yCoord[nodeJ] - yCoord[nodeI];
            double xi = std::hypot(xi_x, xi_y);

            double scBilinear = std::sqrt(characteristicLength / xi) * ft / (9.0 * E);

            maxTensileStrain[nodeI][j] = scBilinear;
        }
    }

    // Initializations
    std::vector<double> accX(totnode); std::vector<double> accY(totnode);
    std::vector<double> velX(totnode); std::vector<double> velY(totnode);
    std::vector<double> velHalfX(totnode); std::vector<double> velHalfY(totnode);
    std::vector<double> dispX(totnode); std::vector<double> dispY(totnode);
    std::vector<double> fIntX(totnode); std::vector<double> fIntY(totnode);
    std::vector<double> fExtX(totnode); std::vector<double> fExtY(totnode);

    std::vector<double> appliedLoading(totalNumLoadStep);
    createLoading
    (
        finalDisplacement, totalNumLoadStep,
        appliedLoading
    );

    // Displacement controlled-loading
    int loadStepNumber = 0; int countBrokenOld = 0;
    double previousLoad = 0.0; double appliedLoad = 0.0;
    initializeForceDispFile
    (
        outputPath
    );

    while (loadStepNumber < totalNumLoadStep)
    {

        for (size_t i = 0; i < dispYassignedNodes.size(); i++)
        {
            int nodeNumber = dispYassignedNodes[i];
            dispY[nodeNumber] = -1.0*appliedLoad;
        }
        
        // Calculate Displacements using Dynamic Relaxation Technique
        int iterationCounter = 0;
        calculateDisplacement
        (
            xCoord, yCoord,
            neighborList,
            effectiveHorizonArea, dtSelected, 
            dispXfixedNodes, dispYfixedNodes,
            dispXassignedNodes, dispYassignedNodes,
            velXassignedNodes, velYassignedNodes,
            forceXassignedNodes, forceYassignedNodes,
            maxTensileStrain, isNeighbor,
            bondHealth, iterationCounter,
            fIntX, fIntY, fExtX, fExtY,
            accX, accY, velX, velY,
            velHalfX, velHalfY, dispX, dispY
        );

        // Calculate the Local Damage AND Permanently Eliminate Tension Capacity of the Broken Bonds
        int countBrokenNew = 0;
        calculateDamage
        (
            xCoord, yCoord,
            neighborList, bondHealth,
            isNeighbor, damageLocal, countBrokenNew
        );
        int numBrokenBond = countBrokenNew - countBrokenOld;

        // Update Maximum Tensile Bond Strains when unloading occurs
        double currentLoad  = dispY[checkDispNode];
        if (fabs(currentLoad) < fabs(previousLoad)) // unloading has occured
        {
            updateMaxTensileStrain
            (
                xCoord, yCoord,
                neighborList,
                dispX, dispY,
                maxTensileStrain
            );
        }
        
        // Write Local Damage Data
        writeDamageData
        (
            outputPath, loadStepNumber, 
            xCoord, yCoord,
            dispX, dispY,
            damageLocal
        );

        // Write Force-Displacement Data
        writeForceDispData
        (
            outputPath, loadStepNumber, 
            iterationCounter, numBrokenBond,
            xCoord, yCoord,
            dispX, dispY, 
            dispXassignedNodes, dispYassignedNodes,
            fIntX, fIntY,
            cmodNodeA, cmodNodeB, checkDispNode
        );
        
        // Log progress
        std::cout   << "Load Step = " << loadStepNumber << '\t'
                    << "# of Iteration = " << iterationCounter << '\t'
                    << "Uapplied = " << (dispY[checkDispNode])*1.0e3 << " mm." << std::endl;
        std::cout << "\tNumber of broken bonds at the Current Loading Step = " << numBrokenBond << "." << std::endl;
        if (iterationCounter > maxIterationNumber) 
        {
            std::cout << std::endl;
            std::cout << "Maximum iteration number has been reached. The solution is NOT aborted." << std::endl;
            std::cout << std::endl;
        }

        // Update Applied Load
        loadStepNumber++;
        appliedLoad = appliedLoading[loadStepNumber];
        
        countBrokenOld = countBrokenNew;
        previousLoad = currentLoad;
        std::cout << endl;
        // end of the current load iteration
    }

    // Record end time
	auto finish = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(finish - start).count();
    fileName = "runFinishInfo.txt";
    writeTimingInfo
    (
        outputPath, fileName, duration
    );
    
    //
	return 0;
}