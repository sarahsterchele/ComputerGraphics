#include "polygonedge.h"

PolygonEdge::PolygonEdge(Vertex p1, Vertex p2)
{
    P1 = p1;
    P2 = p2;

    if (P1.y < P2.y) 
    {
        P1 = p1;
        P2 = p2;
    }
    else
    {
        P1 = p2;
        P2 = p1;
    }

    dx = P2.x - P1.x;
    dy = P2.y - P1.y;
    dz = P2.z - P1.z;
}