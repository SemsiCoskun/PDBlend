// References:
// [1] Silling, Stewart A. Introduction to peridynamics. In Handbook of peridynamic modeling, 63-98.
//          Chapman and Hall/CRC, 2016.

#include "calculateEffectiveHorizonArea.h"

using namespace std;

void calculateEffectiveHorizonArea
(
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers, const int* neighborList,
    double *effectiveHorizonArea
)
{
    // Used for the Silling's volume correction algorithm
    int bondIndex {0};
    for (int nodeI = 0; nodeI < totnode; nodeI++) 
    {
        for (int j = 0; j < neighborNumbers[nodeI]; j++) 
        {
            int nodeJ = neighborList[bondIndex];
            double xi = sqrt(pow((xCoord[nodeJ] - xCoord[nodeI]),2) + pow((yCoord[nodeJ] - yCoord[nodeI]),2));
            double volJ = calculateWeightedVolume (xi);
            effectiveHorizonArea[nodeI] += volJ;

            bondIndex++;
        }
        effectiveHorizonArea[nodeI] += vol;
    }
    // end of effective horizon calculation
}