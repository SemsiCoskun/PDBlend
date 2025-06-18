#include "createNeighbors.h"

void createNeighbors
(
    const double& lengthX, const double& lengthY,
    const std::vector<double>& xCoord, const std::vector<double>& yCoord,
    const ZonesCrackLines& cracks,
    std::vector<std::vector<int>>& neighborList
)
{
    // CellSize = horizon is fixed
    int numCellX = (int) ((lengthX - 0.5*gridSize) / horizon + 1);
    int numCellY = (int) ((lengthY - 0.5*gridSize) / horizon + 1);
    int totNumCell = numCellX*numCellY;

    // Store the list of nodes in each cell
    std::vector<std::vector<int>> cell2nodes(totNumCell);
    for (size_t i = 0; i < xCoord.size(); ++i)
    {
        double x = xCoord[i]; double y = yCoord[i];
        int cellIndX = (int) (x / horizon);
        int cellIndY = (int) (y / horizon);

        int cellIndex = cellIndY + cellIndX * numCellY;
        cell2nodes[cellIndex].push_back(i);
    }
    
    // Store the list of neighboring cells for each node
    std::vector<std::vector<int>> neighborCells(totNumCell);
    for (int cellX = 0; cellX < numCellX; ++cellX) 
    {
        for (int cellY = 0; cellY < numCellY; ++cellY) 
        {
            int cellIndex = cellY + cellX * numCellY;
            for (int dx = -1; dx <= 1; ++dx) 
            {
                int nx = cellX + dx;
                if (nx < 0 || nx >= numCellX) continue;

                for (int dy = -1; dy <= 1; ++dy) 
                {
                    int ny = cellY + dy;
                    if (ny < 0 || ny >= numCellY) continue;

                    int neighborIndex = ny + nx * numCellY;
                    neighborCells[cellIndex].push_back(neighborIndex);
                }
            }
        }
    }

    // Create Neighbor List
    omp_set_num_threads(nThreads);
    #pragma omp parallel for schedule(static)
    for (size_t nodeI = 0; nodeI < xCoord.size(); ++nodeI) 
    {
        double x = xCoord[nodeI];
        double y = yCoord[nodeI];
        int cellX = static_cast<int>(x / horizon);
        int cellY = static_cast<int>(y / horizon);
        int cellIndex = cellY + cellX * numCellY;

        Point nodeA = {xCoord[nodeI], yCoord[nodeI]};

        for (size_t neighborCell : neighborCells[cellIndex]) 
        {
            for (size_t nodeJ : cell2nodes[neighborCell]) 
            {
                if (nodeI == nodeJ) continue;

                double xi_x = xCoord[nodeJ] - x;
                double xi_y = yCoord[nodeJ] - y;
                double xi = std::hypot(xi_x, xi_y);

                if (xi < horizon + 1.0e-3*gridSize)
                {
                    Point nodeB = {xCoord[nodeJ], yCoord[nodeJ]};
                    Line bondAB = {nodeA, nodeB};

                    // Skip the neighbor if crossing crackLines
                    if (doIntersectAnyLine(bondAB, cracks.lines)) continue;

                    neighborList[nodeI].push_back(nodeJ);
                }
            }
        }
        //
    }
    // end
}