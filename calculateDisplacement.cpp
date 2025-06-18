#include "calculateDisplacement.h"

void calculateDisplacement
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const std::vector<std::vector<int>>& neighborList,
    const std::vector<double>& effectiveHorizonArea, const double& dtSelected, 
    const std::vector<int> dispXfixedNodes, const std::vector<int> dispYfixedNodes,
    const std::vector<int> dispXassignedNodes, const std::vector<int> dispYassignedNodes,
    const std::vector<int> velXassignedNodes, const std::vector<int> velYassignedNodes,
    const std::vector<int> forceXassignedNodes, const std::vector<int> forceYassignedNodes,
    const std::vector<std::vector<double>>& maxTensileStrain, const std::vector<std::vector<int>>& isNeighbor,
    std::vector<std::vector<double>>& bondHealth, int& iterationCounter,
    std::vector<double>& fIntX, std::vector<double>& fIntY,std::vector<double>& fExtX, std::vector<double>& fExtY,
    std::vector<double>& accX, std::vector<double>& accY, std::vector<double>& velX, std::vector<double>& velY,
    std::vector<double>& velHalfX, std::vector<double>& velHalfY, std::vector<double>& dispX, std::vector<double>& dispY
)
{
    size_t totnode = xCoord.size();
    std::vector<double> dispNewX(totnode); std::vector<double> dispNewY(totnode);
    std::vector<double> accNewX(totnode); std::vector<double> accNewY(totnode);

    std::vector<double> fIntOldX = fIntX;
    std::vector<double> fIntOldY = fIntY;

    // Displacement Boundary Conditions for the Supports
    for (size_t i = 0; i < dispXfixedNodes.size(); i++)
    {
        int nodeNumber = dispXfixedNodes[i];
        dispX[nodeNumber] = 0.0;
    }
    for (size_t i = 0; i < dispYfixedNodes.size(); i++)
    {
        int nodeNumber = dispYfixedNodes[i];
        dispY[nodeNumber] = 0.0;
    }

    // Iterations to find the stable solution for load step iLoad
    iterationCounter = 1;
    double error = 10;
    while (error > tolerance)
    {
        // First partial velocity update
        for (size_t i {0}; i < xCoord.size(); i++) 
        {
            velHalfX[i] = velHalfX[i] + dtSelected * accX[i];
            velHalfY[i] = velHalfY[i] + dtSelected * accY[i];
        }
        // Enforce velocity boundary conditions
        for (size_t i = 0; i < dispXfixedNodes.size(); i++)
        {
            int nodeNumber = dispXfixedNodes[i];
            velHalfX[nodeNumber] = 0.0;
        }
        for (size_t i = 0; i < dispYfixedNodes.size(); i++)
        {
            int nodeNumber = dispYfixedNodes[i];
            velHalfY[nodeNumber] = 0.0;
        }

        for (size_t i = 0; i < dispXassignedNodes.size(); i++)
        {
            int nodeNumber = dispXassignedNodes[i];
            velHalfX[nodeNumber] = 0.0;
        }
        for (size_t i = 0; i < dispYassignedNodes.size(); i++)
        {
            int nodeNumber = dispYassignedNodes[i];
            velHalfY[nodeNumber] = 0.0;
        }
        
        // Update nodal displacements
        for (size_t i = 0; i < xCoord.size(); i++)
        {
            dispNewX[i] = dispX[i] + dtSelected * velHalfX[i];
            dispNewY[i] = dispY[i] + dtSelected * velHalfY[i];
        }
        
        // Calculate Velocities
        for (size_t i = 0; i < xCoord.size(); i++)
        {
            velX[i] = (dispNewX[i] - dispX[i]) / dtSelected + 0.5 * dtSelected * accX[i];
            velY[i] = (dispNewY[i] - dispY[i]) / dtSelected + 0.5 * dtSelected * accY[i];
        }
        
        // Calculate Internal Forces
        internalForceBondBased
        (
            xCoord, yCoord,
            neighborList,
            effectiveHorizonArea,
            dispNewX, dispNewY, 
            maxTensileStrain, isNeighbor,
            bondHealth, fIntX, fIntY
        );

        // Calculate accelerations
        for (size_t i = 0; i < xCoord.size(); i++)
        {
            accNewX[i] = (fIntX[i] + fExtX[i] - localDamping * velX[i]) / rho;
            accNewY[i] = (fIntY[i] + fExtY[i] - localDamping * velY[i]) / rho;
        }
                
        // Error Tolerance Based on Unbalanced Force at the Location of External Forcing
        double sum1 = 0.0; double sum2 = 0.0;
        for (size_t i = 0; i < dispYassignedNodes.size(); i++)
        {
            int nodeNumber = dispYassignedNodes[i];
            double difference = (fIntOldY[nodeNumber] * vol) - (fIntY[nodeNumber] * vol);
            sum1 += difference*difference;
            sum2 += (fIntY[nodeNumber] * vol)*(fIntY[nodeNumber] * vol);
        }
        error = sqrt(sum1 / sum2);

        // Update variables
        for (size_t i = 0; i < xCoord.size(); i++)
        {
            accX[i] = accNewX[i];
            accY[i] = accNewY[i];
            //
            dispX[i] = dispNewX[i];
            dispY[i] = dispNewY[i];
            //
            fIntOldX[i] = fIntX[i];
            fIntOldY[i] = fIntY[i];
        }        

        if (iterationCounter > maxIterationNumber)
        {
            break;
        }        
        //        
        iterationCounter++;
    }
    //end 
}