#include "writeDamageData.h"

using namespace std;

void writeDamageData
(
    const std::string outputPath, const int& loadStepNumber, 
    const double* xCoord, const double* yCoord,
    const double* dispX, const double* dispY,
    const double* damageLocal
) 
{
    std::ofstream fileDamage(outputPath + "damageData/damage_" + std::to_string(loadStepNumber) + ".csv");
    if (!fileDamage.is_open()) 
    {
        throw runtime_error("Failed to open file for writing damage data");
    }
    
    fileDamage << "2-xDef-coord (mm)" << ',' << "2-yDef-coord (mm)" << ',' 
              << "1-x-coord (mm)" << ',' << "1-y-coord (mm)" << ','
              << "dispX (mm)" << ',' << "dispY (mm)" << ',' << "damage" << '\n';
              
    for (int i = 0; i < totnode; i++) 
    {
        fileDamage << setprecision(6) 
                  << (xCoord[i]+20*dispX[i])*1.0e+3 << ',' << (yCoord[i]+20*dispY[i])*1.0e+3 << ',' 
                  << xCoord[i]*1.0e+3 << ',' << yCoord[i]*1.0e+3 << ',' 
                  << dispX[i]*1.0e+3 << ',' << dispY[i]*1.0e+3 << ',' << damageLocal[i] << '\n';
    }
    fileDamage.close();
}