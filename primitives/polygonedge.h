#ifndef POLYGON_EDGE_H
#define POLYGON_EDGE_H
#include "primitives/vertex.h"

struct PolygonEdge
{
    Vertex P1;
    Vertex P2;
    float dx;
    float dy;
    float dz;
    PolygonEdge(Vertex p1, Vertex p2);
};
#endif