#include "geometricalFeatures.h"

// Function to check if a point is inside a rectangular region
bool isInsideZone(const Point& pt, const Rectangle& zone) 
{
    return pt.x >= zone.bottomLeft.x && pt.x <= zone.topRight.x &&
           pt.y >= zone.bottomLeft.y && pt.y <= zone.topRight.y;
}

// This function checks if the point is inside the region which includes multiple rectangular regions
bool isInsideAnyZone(const Point& pt, const std::vector<Rectangle>& zones) 
{
    for (const auto& zone : zones) 
    {
        if (isInsideZone(pt, zone)) 
        {
            return true; // If inside any zone, return true
        }
    }
    return false; // If not inside any zone, return false
}

// Check the intersection between two lines
bool doIntersect(const Line& bond, const Line& preCrack)
{
    // Data for the first line (i.e. pre-crack line)
    double p1x = preCrack.P.x; // first point x-coord
    double p1y = preCrack.P.y; // first point y-coord
    double p2x = preCrack.Q.x; // second point x-coord
    double p2y = preCrack.Q.y; // second point y-coord
    // Data for the second line (i.e. bond)
    double q1x = bond.P.x;
    double q1y = bond.P.y;
    double q2x = bond.Q.x;
    double q2y = bond.Q.y;
    //
    // Form the Coefficient Matrix
    double A[2][2] = {{p2x-p1x, q1x-q2x}, {p2y-p1y, q1y-q2y}};
    //double b[2] = {q1x-p1x, q1y-p1y};
    // Matrices used for Cramer's Rule
    double A1[2][2] = {{q1x-p1x, q1x-q2x}, {q1y-p1y, q1y-q2y}};
    double A2[2][2] = {{p2x-p1x, q1x-p1x}, {p2y-p1y, q1y-p1y}};
    // Determinants of the Matrices
    double detA = A[0][0] * A[1][1] - A[1][0] * A[0][1];
    double detA1 = A1[0][0] * A1[1][1] - A1[1][0] * A1[0][1];
    double detA2 = A2[0][0] * A2[1][1] - A2[1][0] * A2[0][1];
    // Cramer's Solution
    double t = detA1 / detA;
    double s = detA2 / detA;
    //
    // Check for Intersection
    if ((0 <= t && t <= 1) && (0 <= s && s <= 1)) 
    {
        return true; // "no bond, intersection"
    } else if ((p1x <= q1x && q1x <= p2x) && (p1y <= q1y && q1y <= p2y)) 
    {
        return true; // "no bond, collinear; q1 is inside of [p1,p2]"
    } else if ((p1x <= q2x && q2x <= p2x) && (p1y <= q2y && q2y <= p2y)) 
    {
        return true; // "no bond, collinear; q2 is inside of [p1,p2]"
    } else if ((q1x <= p1x && p1x <= q2x) && (q1y <= p1y && p1y <= q2y)) 
    {
        return true; // "no bond, collinear; p1 is inside of [q1,q2]"
    } else 
    {
        return false; // "proper bond"
    }
}

// Function to check if a line intersects with any of the crack lines
bool doIntersectAnyLine(const Line& bondIJ, const std::vector<Line>& crackLines) 
{
    for (const auto& crackLine : crackLines) 
    {
        if (doIntersect(bondIJ, crackLine)) 
        {
            return true; 
        }
    }
    return false;
}