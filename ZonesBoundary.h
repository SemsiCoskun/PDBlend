#pragma once

#include <cmath>
#include <vector>

#include "geometricalFeatures.h"
#include "globalConstants.h"

struct ZonesBoundary 
{
    Rectangle leftSupport;
    Rectangle rightSupport;
    Rectangle loadingZone;

    ZonesBoundary(double lengthX, double lengthY) 
    {
        leftSupport = 
        Rectangle
        (
            {0.5*(domainX - span) - boundaryLayer, 0.0},
            boundaryLayer, boundaryLayer
        );

        rightSupport = 
        Rectangle
        (
            {0.5*(domainX - span) + span, 0.0},
            boundaryLayer, boundaryLayer
        );

        loadingZone = 
        Rectangle
        (
            {0.5*(domainX - boundaryLayer), lengthY - boundaryLayer},
            boundaryLayer, boundaryLayer
        );
    }

    std::vector<Rectangle> toVector() const 
    {
        return {leftSupport, rightSupport, loadingZone};
    }
};