// References:
// [1] W. Hu, Y. D. Ha, F. Bobaru. Numerical integration in peridynamics. 
//      Technical Report. University of Nebraska-Lincoln. 2010.

#include "calculatePartialAreas.h"

using namespace std;

double calculatePartialAreas
(
    const double& xi
)
{
    // Partial Area Calculations
    double smallIncrement = 1.0e-3 * gridSize;
    double volJ {};
    // Compute volumes
    if ((xi + 0.5 * gridSize) <= (horizon + smallIncrement)) 
    {
        volJ = vol;
    } else if ((xi - 0.5 * gridSize) <= (horizon + smallIncrement)) 
    {
        volJ = (1.0 / gridSize) * (horizon - (xi - 0.5 * gridSize)) * vol;
    } else 
    {
        volJ = 0.0;
    }
    return volJ;
    // end of the partial volume calculation
}