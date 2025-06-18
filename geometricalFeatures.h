#ifndef GEOMETRICALFEATURES_H
#define GEOMETRICALFEATURES_H

#include <vector>
#include <cmath>

struct Point 
{
    double x;
    double y;

    Point() = default;
};

struct Line
{
    Point P;
    Point Q;

    Line() = default;

    Line(const Point& p1, const Point& p2) : P(p1), Q(p2) {}
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
    Line bottomEdge, rightEdge, topEdge, leftEdge;

    // Default constructor
    Rectangle() = default;

    Rectangle(const Point& bl, double lengthX, double lengthY)
        : bottomLeft(bl), lengthZoneX(lengthX), lengthZoneY(lengthY) 
    {
        // Initialize corner points
        bottomRight = {bottomLeft.x + lengthZoneX, bottomLeft.y};
        topRight = {bottomRight.x, bottomRight.y + lengthZoneY};
        topLeft = {bottomLeft.x, topRight.y};

        // Initialize edges as Line segments between the corners
        bottomEdge = {bottomLeft, bottomRight};  
        rightEdge = {bottomRight, topRight};    
        topEdge = {topRight, topLeft};       
        leftEdge = {topLeft, bottomLeft};      
        
    }
};

bool isInsideZone(const Point& pt, const Rectangle& zone);
bool isInsideAnyZone(const Point& pt, const std::vector<Rectangle>& zones);
bool doIntersect(const Line& bond, const Line& preCrack);
bool doIntersectAnyLine(const Line& bondIJ, const std::vector<Line>& crackLines);

#endif 