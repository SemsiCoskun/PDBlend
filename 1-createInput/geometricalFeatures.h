#ifndef GEOMETRICALFEATURES_H
#define GEOMETRICALFEATURES_H

#include <vector>
#include <cmath>

struct Point 
{
    double x;
    double y;
};

struct Line
{
    Point P;
    Point Q;
};

struct Rectangle 
{
    Point bottomLeft;
    double lengthZoneX;
    double lengthZoneY;

    Point bottomRight;
    Point topRight;
    Point topLeft;

    // Define edges as Line segments
    Line l1, l2, l3, l4;

    Rectangle(const Point& bl, double lengthX, double lengthY)
        : bottomLeft(bl), lengthZoneX(lengthX), lengthZoneY(lengthY) {
        
        // Initialize corner points
        bottomRight = {bottomLeft.x + lengthZoneX, bottomLeft.y};
        topRight = {bottomRight.x, bottomRight.y + lengthZoneY};
        topLeft = {bottomLeft.x, topRight.y};

        // Initialize edges as Line segments between the corners
        l1 = {bottomLeft, bottomRight};  // Bottom edge
        l2 = {bottomRight, topRight};    // Right edge
        l3 = {topRight, topLeft};        // Top edge
        l4 = {topLeft, bottomLeft};      // Left edge
        
    }
};

bool isInsideZone(const Point& pt, const Rectangle& zone);
bool isInsideAnyZone(const Point& pt, const std::vector<Rectangle>& zones);
bool doIntersect(const Line& bond, const Line& preCrack);
bool doIntersectAnyLine(const Line& bondIJ, const std::vector<Line>& crackLines);

#endif 