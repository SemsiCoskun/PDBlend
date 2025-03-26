// References:
// [1] ADD ARCHIVE REFERENCE HERE

#include "assignBondConstants.h"

using namespace std;

const double pi = 3.1415926535897932384626433832795028841971;

void assignBondConstants
(
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers, const int* neighborList,
    const double* effectiveHorizonArea,
    const double* youngsModulus, const double* tensileStrength, const double* fractureEnergy,
    double* bondConstant, double* criticalBondStrainLinear,
    double* criticalBondStrainBilinear, double* failureBondStrainBilinear
)
{
    double vol_0 = pi * horizon * horizon * thickness; // full neighborhood volume
    int bondIndex {0};
    for (int nodeI = 0; nodeI < totnode; nodeI++) 
    {
        for (int j = 0; j < neighborNumbers[nodeI]; j++) 
        {
            int nodeJ = neighborList[bondIndex];
            double vol_i = effectiveHorizonArea[nodeI];
            double vol_j = effectiveHorizonArea[nodeJ];

            double xi = sqrt(pow((xCoord[nodeJ] - xCoord[nodeI]),2) + pow((yCoord[nodeJ] - yCoord[nodeI]),2));

            // Material constants are selected as the average of the values at nodeI and nodeJ, except for the fracture energy.
            // This can be modified, for example taking weighted average etc.
            double bondYoungsModulus = 0.5 * (youngsModulus[nodeI] + youngsModulus[nodeJ]);
            double bondTensileStrength = 0.5 * (tensileStrength[nodeI] + tensileStrength[nodeJ]);
            double bondFractureEnergy = 0.0;
            if (fractureEnergy[nodeI] < fractureEnergy[nodeJ]) 
            {
                bondFractureEnergy = fractureEnergy[nodeI];
            } else
            {
                bondFractureEnergy =fractureEnergy[nodeJ];
            }
            // Bond Constant:
            double bc = (2.0 * vol_0 / (vol_i + vol_j)) * 9.0 * bondYoungsModulus / (pi * thickness * pow(horizon, 3));
            // Critical Bond-Strain for the linear constititive law
            double scLinear = sqrt(4.0 * pi * bondFractureEnergy / (9.0 * bondYoungsModulus * horizon));
            // Critical Bond-Strain for the bilinear constititive law (the onset of nonlinearity)
            double scBilinear = sqrt(characteristicLength / xi) * bondTensileStrength / (9.0 * bondYoungsModulus);
            // Failure Bond-Strain for the bilinear constititive law
            double sfBilinear = (3.0 * pi * bondFractureEnergy / (sqrt(characteristicLength * xi) * bondTensileStrength));

            // Insert computed values for each bond
            bondConstant[bondIndex] = bc;
            criticalBondStrainLinear[bondIndex] = scLinear;
            criticalBondStrainBilinear[bondIndex] = scBilinear;
            failureBondStrainBilinear[bondIndex] = sfBilinear;
            //
            bondIndex++;
        }
    }
    // end
}