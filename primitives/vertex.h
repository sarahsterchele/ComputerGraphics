#ifndef VERTEX_H
#define VERTEX_H
#include "primitives/color.h"
#include "primitives/normal.h"

struct Vertex
{
    int x;
    int y;
    float z;
    float world_x;
    float world_y;
    float world_z;
    Color color;
    Normal normal;
    bool normalDefined;

    Vertex(){};
    Vertex(int x, int y, float z, float world_x, float world_y, float world_z, Color color, bool normalDefined, Normal normal): 
        x(x),
        y(y), 
        z(z), 
        world_x(world_x), 
        world_y(world_y), 
        world_z(world_z), 
        color(color),
        normal(normal),
        normalDefined(normalDefined)
    {}
};
#endif