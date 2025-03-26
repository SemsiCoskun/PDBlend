#include "calculateDamage.h"

using namespace std;

void calculateDamage
(
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers, const int* neighborList,
    const double* bondHealth, int* isNeighbor,
    double* damageLocal, int &countBrokenNew
)
{
    countBrokenNew = 0;
    int bondIndex {0};
    for (int nodeI = 0; nodeI < totnode; nodeI++) 
    {
        double neighborArea = 0.0;
        double initialArea = 0.0;
        damageLocal[nodeI] = 0.0;
        for (int j = 0; j < neighborNumbers[nodeI]; j++) 
        {
            int nodeJ = neighborList[bondIndex];
            double xi = sqrt(pow((xCoord[nodeJ] - xCoord[nodeI]),2) + pow((yCoord[nodeJ] - yCoord[nodeI]),2));
            double volJ = calculateWeightedVolume (xi);

            if (bondHealth[bondIndex] < 0.0001)
            {
                isNeighbor[bondIndex] = 0;
                countBrokenNew++;
            }
            
            initialArea += volJ;
            neighborArea += isNeighbor[bondIndex] * bondHealth[bondIndex] * volJ;            
            //
            bondIndex++;
        }
        damageLocal[nodeI] = 1.0 - neighborArea / initialArea; 
    }
    // end
}