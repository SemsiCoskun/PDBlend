#pragma once

#include <cmath>
#include <vector>

#include "geometricalFeatures.h"
#include "globalConstants.h"
#include "ZonesExclusion.h"
#include "ZonesBoundary.h"

struct ZonesCrackLines 
{
    std::vector<Line> lines;

    ZonesCrackLines
    (
        double lengthX, double lengthY,
        const ZonesExclusion& exclusion,
        const ZonesBoundary& boundary
    ) 
    {
        Point start = {0.5 * lengthX, boundaryLayer};
        Point end   = {0.5 * lengthX, start.y + crackLength};

        lines.push_back(Line(start, end)); // prenotch

        // Add lines from zones
        lines.insert
        (
            lines.end(), 
            {
                exclusion.subtractZone1.topEdge,
                boundary.leftSupport.leftEdge,
                boundary.leftSupport.rightEdge,

                exclusion.subtractZone2.topEdge,
                boundary.rightSupport.leftEdge,
                boundary.rightSupport.rightEdge,

                exclusion.subtractZone3.topEdge,
                exclusion.subtractZone5.bottomEdge,
                exclusion.subtractZone4.bottomEdge,

                boundary.loadingZone.leftEdge,
                boundary.loadingZone.rightEdge,
            }
        );
    }
};