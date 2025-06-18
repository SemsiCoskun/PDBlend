#pragma once

#include <cmath>
#include <vector>

#include "geometricalFeatures.h"
#include "globalConstants.h"

struct ZonesExclusion
{
    Rectangle subtractZone1;
    Rectangle subtractZone2;
    Rectangle subtractZone3;
    Rectangle subtractZone4;
    Rectangle subtractZone5;

    ZonesExclusion(double lengthX, double lengthY) 
    {
        subtractZone1 = 
        Rectangle
        (
            {0.0, 0.0}, 
            0.5*(domainX - span) - boundaryLayer, boundaryLayer
        );

        subtractZone2 = 
        Rectangle
        (
            {0.5*(domainX - span), 0.0},
            span, boundaryLayer
        );

        subtractZone3 = 
        Rectangle
        (
            {domainX - 0.5*(domainX - span) + boundaryLayer, 0.0},
            0.5*(domainX - span) - boundaryLayer, boundaryLayer
        );

        subtractZone4 = 
        Rectangle
        (
            {0.0, lengthY - boundaryLayer},
            0.5 * (lengthX - boundaryLayer), boundaryLayer
        );

        subtractZone5 = 
        Rectangle
        (
            {0.5 * lengthX + 0.5 * boundaryLayer, lengthY - boundaryLayer},
            0.5 * (lengthX - boundaryLayer), boundaryLayer
        );
    }

    std::vector<Rectangle> toVector() const 
    {
        return {subtractZone1, subtractZone2, subtractZone3, subtractZone4, subtractZone5};
    }
};