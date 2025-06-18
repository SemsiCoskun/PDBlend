#include "createMesh.h"

void createMesh
(
    const double& lengthX, const double& lengthY,
    const ZonesExclusion& exclusion,
    std::vector <double>& xCoord, std::vector <double>& yCoord
)
{
    int ndivx = (int) (lengthX / gridSize);
    int ndivy = (int) (lengthY / gridSize);
    int totnode = ndivx * ndivy; // for the greater domain

    std::vector <double> xGreater(totnode);
    std::vector <double> yGreater(totnode);
    int countNode = 0;
    for (int i = 0; i < ndivx; ++i) 
    {
        double x = 0.5*gridSize + i * gridSize;
        for (int j = 0; j < ndivy; j++) 
        {
            double y = 0.5*gridSize + j * gridSize;
            xGreater[countNode] = x;
            yGreater[countNode] = y;
            countNode++;
        }
    }

    // Assign coordinates of the material points
    for (int i = 0; i < ndivx; i++) 
    {
        for (int j = 0; j < ndivy; j++) 
        {
            Point pt = {0.5*gridSize + i * gridSize, 0.5*gridSize + j * gridSize};
            
            // Skip nodes in any of the exclusion zones
            if (isInsideAnyZone(pt, exclusion.toVector())) 
            {
                continue;
            }

            // Add coordinates to vectors
            xCoord.push_back(pt.x);
            yCoord.push_back(pt.y);
        }
    }
    // end
}