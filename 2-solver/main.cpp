// References:
// [1] D. Littlewood. Roadmap for software implementation. In Handbook of peridynamic modeling, pages 147–178.
//          Chapman and Hall/CRC, 2016.
// [2] Silling, S., 2000. Reformulation of elasticity theory for discontinuities and long-range forces. Journal of the
//  Mechanics and Physics of Solids 48, 175–209. doi:https://doi.org/10.1016/S0022-5096(99)00029-0.

#include <iostream>
#include <fstream>
#include <chrono>  // for high_resolution_clock
// To make directories
#include <sys/stat.h>
#include <sys/types.h>

#include "globalConstants.h"
#include "enterInputs.h"
#include "calculateWeightedVolume.h"
#include "calculateEffectiveHorizonArea.h"
#include "assignBondConstants.h"
#include "calculateDamage.h"
#include "calculateDt.h"
#include "internalForceBondBasedBilinear.h"
#include "createLoading.h"
#include "calculateDisplacement.h"
#include "updateMaxBondStrain.h"
#include "writeNodeIdentificationData.h"
#include "writeLoadingData.h"
#include "writeNeighborNumberData.h"
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

    std::string inputPath = "../3-IO/input/";
    std::string outputPath = "../3-IO/output/";

    // Create a folder to store the output data
    mkdir("../3-IO/output/damageData", 0777);
    
    // Create coordinate arrays
    double xCoord[totnode]{};
    double yCoord[totnode]{}; 
    // Create boundary node arrays
    int dispXfixedNodes[numNodeDispXfixed]{};    
    int dispYfixedNodes[numNodeDispYfixed]{};
    int dispXassignedNodes[numNodeDispXassigned]{};
    int dispYassignedNodes[numNodeDispYassigned]{};    
    int forceXassignedNodes[numNodeForceXassigned]{};
    int forceYassignedNodes[numNodeForceYassigned]{};
    // Create neighborhood arrays
    int neighborNumbers[totnode]{};
    // Dynamic allocation will be used (beacuse stack memory is limited, use dynamic allocation to create arrays which are large in size.)
    int* neighborList = new int[sizeNeighborList]{};
    
    // Enter inputs:
    enterInputs
    (
        inputPath, xCoord, yCoord,
        dispXfixedNodes, dispYfixedNodes,
        dispXassignedNodes, dispYassignedNodes,
        forceXassignedNodes, forceYassignedNodes,
        neighborNumbers, neighborList
    );

    // For checking purposes:
    writeNodeIdentificationData
    (
        outputPath, xCoord, yCoord, 
        dispXfixedNodes, dispYfixedNodes,
        dispXassignedNodes, dispYassignedNodes,
        forceXassignedNodes, forceYassignedNodes
    );
    writeNeighborNumberData
    (
        outputPath, 
        xCoord, yCoord,
        neighborNumbers
    );

    // Calculate effective horizon areas to apply surface corrections
    double effectiveHorizonArea[totnode]{};
    calculateEffectiveHorizonArea
    (
        xCoord, yCoord,
        neighborNumbers, neighborList,
        effectiveHorizonArea
    );

    // Material Constants at Nodes
    double youngsModulus[totnode]{};
    double tensileStrength[totnode]{};
    double fractureEnergy[totnode]{};
    // Here, it is assumed that the domain is made of single material. 
    // Any modification is possible to provide the distribution of the material constants.
    for (int i = 0; i < totnode; i++)
    {
        youngsModulus[i] = E;
        tensileStrength[i] = ft;
        fractureEnergy[i] = Gf;
    }

    // Bond Constants
    double* bondConstant = new double[sizeNeighborList]{};
    double* criticalBondStrainLinear = new double[sizeNeighborList]{};
    double* criticalBondStrainBilinear = new double[sizeNeighborList]{};
    double* failureBondStrainBilinear = new double[sizeNeighborList]{};    
    assignBondConstants
    (
        xCoord, yCoord,
        neighborNumbers, neighborList,
        effectiveHorizonArea,
        youngsModulus, tensileStrength, fractureEnergy,
        bondConstant, criticalBondStrainLinear,
        criticalBondStrainBilinear, failureBondStrainBilinear
    );

    double* currentBondStrain = new double[sizeNeighborList]{};
    for (int i = 0; i < sizeNeighborList; i++)
    {
        currentBondStrain[i] = 0.0;
    }
    // When there is unloading, maxTensileBondStrain > bondStrain
    double* maxTensileBondStrain = new double[sizeNeighborList];
    for (int i = 0; i < sizeNeighborList; i++)
    {
        maxTensileBondStrain[i] = criticalBondStrainBilinear[i];
    }
    // bondHealth is controlled by the bond stiffness. if there exist a softening: bondHealth < 1.0
    // bondHealth[bond]==1 -> intact bond
    double* bondHealth = new double[sizeNeighborList]; // a double btw 0.0-1.0
    for (int i = 0; i < sizeNeighborList; i++)
    {
        bondHealth[i] = 1.0;
    }
    // isNeighbor indicates whether the bond's tensile force carrying capacity is permanently eliminated or not
    // isNeighbor[bond]== 0 -> the bond cannot carry tension anymore.
    int* isNeighbor = new int[sizeNeighborList]; // either 0 or 1
    for (int i = 0; i < sizeNeighborList; i++)
    {
        isNeighbor[i] = 1;
    }
    // damageLocal provides the ratio of number of broken bonds (i.e., bonds cannot carry tension anymore) 
    // to the initial number of bonds attached to the material point i
    // damageLocal[i]==0 -> all the bonds connected to the node i is broken.
    double damageLocal[totnode]; // a double btw 0.0-1.0
    for (int i = 0; i < totnode; i++)
    {
        damageLocal[i] = 0.0; 
    }
    
    // Time Step Calculation
    double dtSelected = calculateDt
                        (
                            xCoord, yCoord,
                            neighborNumbers, neighborList,
                            bondConstant
                        );
    std::cout << std::endl;
    std::cout << "dtSelected = " << dtSelected << " seconds." << std::endl;
    std::cout << std::endl;

    // Initializations
    double* velX = new double[totnode]{}; double* velHalfX = new double[totnode]{};
    double* velY = new double[totnode]{}; double* velHalfY = new double[totnode]{};

    double* accX = new double[totnode]{}; double* accY = new double[totnode]{};
    double* dispX = new double[totnode]{}; double* dispY = new double[totnode]{};

    double* fIntX = new double[totnode]{}; double* fIntY = new double[totnode]{};
    double* fExtX = new double[totnode]{}; double* fExtY = new double[totnode]{};

    double appliedLoading[totalNumLoadStep]{};
    createLoading
    (
        finalDisplacement, totalNumLoadStep,
        appliedLoading
    );
    writeLoadingData
    (
        outputPath, appliedLoading
    );
    
    // Displacement controlled-loading up to failure
    int loadStepNumber = 0; int countBrokenOld = 0;
    double previousLoad = 0.0; double appliedLoad = 0.0;
    initializeForceDispFile
    (
        outputPath
    );
    while (loadStepNumber <= totalNumLoadStep)
    {

        for (int i = 0; i < numNodeDispYassigned; i++)
        {
            int nodeNumber = dispYassignedNodes[i];
            dispY[nodeNumber] = -1.0*appliedLoad;
        }

        double currentLoad  = dispY[checkDispNode];
        if (fabs(currentLoad) < fabs(previousLoad)) // unloading has occured
        {
            // Update Maximum Tensile Bond Strains
            updateMaxBondStrain
            (
                currentBondStrain, criticalBondStrainBilinear,
                failureBondStrainBilinear, maxTensileBondStrain
            );
        }
        
        // Calculate Displacements using Dynamic Relaxation Technique
        int iterationCounter = 0;
        calculateDisplacement
        (
            xCoord, yCoord, 
            neighborNumbers, neighborList,
            dispXfixedNodes, dispYfixedNodes,
            dispXassignedNodes, dispYassignedNodes,
            forceXassignedNodes, forceYassignedNodes,
            bondConstant, criticalBondStrainBilinear,
            failureBondStrainBilinear, maxTensileBondStrain, 
            isNeighbor, dtSelected,
            fIntX, fIntY, fExtX, fExtY,
            accX, accY, velX, velY,
            velHalfX, velHalfY, dispX, dispY,
            currentBondStrain, bondHealth, iterationCounter
        );

        // Calculate the Local Damage AND Permanently Eliminate Tension Capacity of the Broken Bonds
        int countBrokenNew = 0;
        calculateDamage
        (
            xCoord, yCoord,
            neighborNumbers, neighborList,
            bondHealth, isNeighbor,
            damageLocal, countBrokenNew
        );
        int numBrokenBond = countBrokenNew - countBrokenOld;

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
            fIntX, fIntY
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

    // Deallocate the memory
    delete[] neighborList;
    delete[] bondConstant;
    delete[] criticalBondStrainLinear;
    delete[] criticalBondStrainBilinear;
    delete[] failureBondStrainBilinear;
    delete[] maxTensileBondStrain;
    delete[] currentBondStrain;
    delete[] bondHealth;
    delete[] isNeighbor;
    delete[] velX; delete[] velHalfX;
    delete[] velY; delete[] velHalfY;
    delete[] accX; delete[] accY;
    delete[] dispX; delete[] dispY;
    delete[] fIntX; delete[] fIntY;
    delete[] fExtX; delete[] fExtY;

    // Record end time
	auto finish = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(finish - start).count();
    writeTimingInfo
    (
        outputPath, duration
    );
    //
	return 0;
}