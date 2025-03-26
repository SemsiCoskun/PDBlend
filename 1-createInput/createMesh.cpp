#include "createMesh.h"

using namespace std;

void createMesh
(
    const double& lengthX, const double& lengthY, 
    const double& gridSize, const double& horizon,
    const std::vector<Rectangle>& exclusionZones,
    std::vector<double>& xCoord, std::vector<double>& yCoord
)
{
    int ndivx = lengthX / gridSize;
    int ndivy = lengthY / gridSize;

    // Assign coordinates of the material points
    for (int i = 0; i < ndivx; i++) 
    {
        for (int j = 0; j < ndivy; j++) 
        {
            Point pt = {-0.5 * (lengthX - gridSize) + i * gridSize, -0.5 * (lengthY - gridSize) + j * gridSize};
            
            // Skip nodes in any of the exclusion zones
            if (isInsideAnyZone(pt, exclusionZones)) 
            {
                continue;
            }

            // Add coordinates to vectors
            xCoord.push_back(pt.x);
            yCoord.push_back(pt.y);
        }
    }
    //
}