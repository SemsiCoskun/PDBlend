// References:
// [1] Underwood, P., 1983. Dynamic relaxation, in: Computational Methods in Mechanics, Vol 1: Computational
//          methods for transient analysis. Elsevier Science Publishers B.V., pp. 245–265.
// [2] D. Littlewood. Roadmap for software implementation. In Handbook of peridynamic modeling, pages 147–178.
//          Chapman and Hall/CRC, 2016.
// [3] Silling, S.A., Askari, E., 2005. A meshfree method based on the peridynamic model of solid mechanics.
//          Computers & structures 83, 1526–1535. doi:https://doi.org/10.1016/j.compstruc.2004.11.026.

#include "calculateDisplacement.h"

void calculateDisplacement
(
    const double* xCoord, const double* yCoord, 
    const int* neighborNumbers, const int* neighborList,
    const int* dispXfixedNodes, const int* dispYfixedNodes,
    const int* dispXassignedNodes, const int* dispYassignedNodes,
    const int* forceXassignedNodes, const int* forceYassignedNodes,
    const double* bondConstant, const double* criticalBondStrainBilinear,
    const double* failureBondStrainBilinear, const double* maxTensileBondStrain, 
    const int* isNeighbor, const double& dtSelected,
    double* fIntX, double* fIntY, double *fExtX, double *fExtY,
    double *accX, double *accY, double *velX, double *velY,
    double *velHalfX, double *velHalfY, double *dispX, double *dispY,
    double* currentBondStrain, double* bondHealth, int &iterationCounter
)
{
    double dispNewX[totnode]; double dispNewY[totnode];
    double accNewX[totnode]; double accNewY[totnode];
    double fIntOldX[totnode]; double fIntOldY[totnode];
    for (int i = 0; i < totnode; i++)
    {
        dispNewX[i] = 0.0; dispNewY[i] = 0.0;
        accNewX[i] = 0.0; accNewY[i] = 0.0;
        fIntOldX[i] = fIntX[i];
        fIntOldY[i] = fIntY[i];
    }
    // Displacement Boundary Conditions for the Supports
    for (int i = 0; i < numNodeDispXfixed; i++)
    {
        int nodeNumber = dispXfixedNodes[i];
        dispX[nodeNumber] = 0.0;
    }
    for (int i = 0; i < numNodeDispYfixed; i++)
    {
        int nodeNumber = dispYfixedNodes[i];
        dispY[nodeNumber] = 0.0;
    }

    // Iterations to find the stable solution for the load steps
    iterationCounter = 1;
    double error = 10;
    while (error > tolerance)
    {
        // First partial velocity update
        for (int i {0}; i < totnode; i++) 
        {
            velHalfX[i] = velHalfX[i] + dtSelected * accX[i];
            velHalfY[i] = velHalfY[i] + dtSelected * accY[i];
        }
        // Enforce velocity boundary conditions
        for (int i = 0; i < numNodeDispXfixed; i++)
        {
            int nodeNumber = dispXfixedNodes[i];
            velHalfX[nodeNumber] = 0.0;
        }
        for (int i = 0; i < numNodeDispYfixed; i++)
        {
            int nodeNumber = dispYfixedNodes[i];
            velHalfY[nodeNumber] = 0.0;
        }

        for (int i = 0; i < numNodeDispXassigned; i++)
        {
            int nodeNumber = dispXassignedNodes[i];
            velHalfX[nodeNumber] = 0.0;
        }
        for (int i = 0; i < numNodeDispYassigned; i++)
        {
            int nodeNumber = dispYassignedNodes[i];
            velHalfY[nodeNumber] = 0.0;
        }
        
        // Update nodal displacements
        for (int i = 0; i < totnode; i++)
        {
            dispNewX[i] = dispX[i] + dtSelected * velHalfX[i];
            dispNewY[i] = dispY[i] + dtSelected * velHalfY[i];
        }
        
        // Calculate Velocities
        for (int i = 0; i < totnode; i++)
        {
            velX[i] = (dispNewX[i] - dispX[i]) / dtSelected + 0.5 * dtSelected * accX[i];
            velY[i] = (dispNewY[i] - dispY[i]) / dtSelected + 0.5 * dtSelected * accY[i];
        }
        
        // Calculate Internal Forces
        internalForceBondBasedBilinear
        (
            xCoord, yCoord, 
            neighborNumbers, neighborList,
            bondConstant, criticalBondStrainBilinear,
            failureBondStrainBilinear, maxTensileBondStrain, 
            isNeighbor, dispNewX, dispNewY,
            currentBondStrain, bondHealth, fIntX, fIntY
        );

        // Calculate accelerations
        for (int i = 0; i < totnode; i++)
        {
            accNewX[i] = (fIntX[i] + fExtX[i] - localDamping * velX[i]) / rho;
            accNewY[i] = (fIntY[i] + fExtY[i] - localDamping * velY[i]) / rho;
        }
                
        // Error Tolerance Based on Unbalanced Force at the Location of External Forcing
        double sum1 = 0.0; double sum2 = 0.0;

        double errorX = 0.0;
        for (int i = 0; i < numNodeDispXassigned; i++)
        {
            int nodeNumber = dispXassignedNodes[i];
            double difference = (fIntOldX[nodeNumber] * vol) - (fIntX[nodeNumber] * vol);
            sum1 += difference*difference;
            sum2 += (fIntX[nodeNumber] * vol)*(fIntX[nodeNumber] * vol);
        }
        errorX = sqrt(sum1 / sum2);

        sum1 = 0.0; sum2 = 0.0;
        double errorY = 0.0;
        for (int i = 0; i < numNodeDispYassigned; i++)
        {
            int nodeNumber = dispYassignedNodes[i];
            double difference = (fIntOldY[nodeNumber] * vol) - (fIntY[nodeNumber] * vol);
            sum1 += difference*difference;
            sum2 += (fIntY[nodeNumber] * vol)*(fIntY[nodeNumber] * vol);
        }
        errorY = sqrt(sum1 / sum2);

        if (errorX > errorY)
        {
            error = errorX;
        } else
        {
            error = errorY;
        }
        
        // Update variables
        for (int i = 0; i < totnode; i++)
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