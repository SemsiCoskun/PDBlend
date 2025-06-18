#include "createBoundary.h"

void createBoundary
(
    const double& lengthX, const double& lengthY,
    const std::vector <double>& xCoord, const std::vector <double>& yCoord,
    const ZonesBoundary& boundary,
    std::vector<int>& dispXfixedNodes, std::vector<int>& dispYfixedNodes,
    std::vector<int>& dispXassignedNodes, std::vector<int>& dispYassignedNodes,
    std::vector<int>& velXassignedNodes, std::vector<int>& velYassignedNodes,
    std::vector<int>& forceXassignedNodes, std::vector<int>& forceYassignedNodes,
    std::vector<int>& nodeIdentification, int& cmodNodeA, int& cmodNodeB, int& checkDispNode
)
{
    int boundType = 1;
    // Left support
    for (size_t i = 0; i < xCoord.size(); i++)
    {
        Point node = {xCoord[i], yCoord[i]};
        // Include the nodes in the list
        if (isInsideZone(node, boundary.leftSupport)) 
        {
            dispXfixedNodes.push_back(i);
            dispYfixedNodes.push_back(i);
            nodeIdentification[i] = boundType;
        }
    }
    boundType++;

    // Right support
    for (size_t i = 0; i < xCoord.size(); i++)
    {
        Point node = {xCoord[i], yCoord[i]};
        // Include the nodes in the list
        if (isInsideZone(node, boundary.rightSupport)) 
        {
            dispYfixedNodes.push_back(i);
            nodeIdentification[i] = boundType;
        }
    }
    boundType++;

    // loading
    for (size_t i = 0; i < xCoord.size(); i++)
    {
        Point node = {xCoord[i], yCoord[i]};
        // Include the nodes in the list
        if (isInsideZone(node, boundary.loadingZone)) 
        {
            dispYassignedNodes.push_back(i);
            nodeIdentification[i] = boundType;
        }
    }
    boundType++;

    // Nodes to measure crack mount opening distance
    for (size_t i = 0; i < xCoord.size(); i++)
    {
        if ((fabs(xCoord[i] - 0.5*lengthX) <= 0.65*gridSize) && (fabs(yCoord[i] - boundaryLayer) <= 0.65*gridSize) && (xCoord[i] < 0.5*lengthX))
        {
            cmodNodeA = i;
            nodeIdentification[i] = boundType;
        }       
    }
    boundType++;

    for (size_t i = 0; i < xCoord.size(); i++)
    {
        if ((fabs(xCoord[i] - 0.5*lengthX) <= 0.65*gridSize) && (fabs(yCoord[i] - boundaryLayer) <= 0.65*gridSize) && (xCoord[i] > 0.5*lengthX))
        {
            cmodNodeB = i;
            nodeIdentification[i] = boundType;
        }        
    }
    boundType++;

    // Node to measure the reaction for force-disp output data
    checkDispNode = dispYassignedNodes[floor(0.5*dispYassignedNodes.size())];
    nodeIdentification[checkDispNode] = boundType;
}