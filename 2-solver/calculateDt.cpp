// References:
// [1] Silling, S.A., Askari, E., 2005. A meshfree method based on the peridynamic model of solid mechanics.
//          Computers & structures 83, 1526–1535. doi:https://doi.org/10.1016/j.compstruc.2004.11.026.

#include "calculateDt.h"

using namespace std;

double calculateDt
(
    const double* xCoord, const double* yCoord,
    const int* neighborNumbers, const int* neighborList,
    const double* bondConstant
)
{
    double dtCritical {0.0};
	double dtCriticalMin {1.0}; // minimum critical dt throughout the domain
    double sumTcr = 0.0;
    int bondIndex {0};
    for (int nodeI = 0; nodeI < totnode; nodeI++) 
    {
        sumTcr = 0.0;
        for (int j = 0; j < neighborNumbers[nodeI]; j++) 
        {
            int nodeJ = neighborList[bondIndex];
            double xi = sqrt(pow((xCoord[nodeJ] - xCoord[nodeI]),2) + pow((yCoord[nodeJ] - yCoord[nodeI]),2));
            double volJ = calculateWeightedVolume (xi);
            
            double bc = bondConstant[bondIndex];
            sumTcr += bc / xi * volJ;
            //
            bondIndex++;
        }
        dtCritical = sqrt(2.0 * rho / sumTcr);		
		if (dtCritical < dtCriticalMin) 
        {
			dtCriticalMin = dtCritical;
		} 
    }
    return (dtSF * dtCriticalMin);
    // end of the time step calculation
}