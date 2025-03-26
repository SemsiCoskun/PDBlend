#include "createNeighbors.h"

using namespace std;

void createNeighbors
(
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const int& totnode, const double& horizon, const double& gridSize,
    const std::vector<Line>& crackLines,
    std::vector<std::vector<int>>& neighbors
)
{
    double smallIncrement = 1.0e-3 * gridSize;
    for (int i = 0; i < totnode; i++)
    {
        Point nodeI = {xCoord[i], yCoord[i]};
        for (int j = 0; j < totnode; j++) 
        {
            if (i == j) continue;  // Skip self-comparison

            double xi = sqrt(pow((xCoord[j] - xCoord[i]),2) + pow((yCoord[j] - yCoord[i]),2));

            if ((xi - 0.5 * gridSize <= (horizon + smallIncrement))) 
            {
                Point nodeJ = {xCoord[j], yCoord[j]};
                Line bondIJ = {nodeI, nodeJ};

                // Skip the neighbor if the bond intersects with any crack line
                if (doIntersectAnyLine(bondIJ, crackLines)) continue;

                // Add j as a neighbor of i
                neighbors[i].push_back(j);
            }
        }
    }
    // end
}