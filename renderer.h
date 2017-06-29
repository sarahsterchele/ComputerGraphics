#ifndef RENDERER_H
#define RENDERER_H

#include <cstddef>
#include "shader.h"
#include "z_buffer.h"
#include "application/drawable.h"
#include "primitives/polygonedge.h"
#include "primitives/scanline.h"

class Renderer
{
public:
    Renderer(Drawable *drawable, Z_Buffer *zbuffer, Shader *shader);
    void draw_Line(Vertex p1, Vertex p2);
    void draw_Polygon(Vertex p1, Vertex p2, Vertex p3, bool isFilled = true);
    void draw_Mesh(std::vector<std::vector<Vertex> > mesh, int rows, int cols, bool filledIn);
    Color lerpColor(Color &color1, Color &color2, float t);
    float lerp(float p1, float p2, float t);
    std::vector<float> lerpVector(float x1, float y1, float z1, float x2, float y2, float z2, float t);
    std::vector<float> lerpVector(std::vector<float> p1, std::vector<float> p2, float t);

private:    
    Drawable *drawable;
    Z_Buffer *zbuffer;
    Shader *shader;
    void draw_Scanline(Scanline &scanLine, float y);
    void drawScanlinesBetweenEdges(PolygonEdge &e1, PolygonEdge &e2, Color midpointColor);
    bool isVertexViewable(int x, int y, float z);
    bool isLineViewable(Vertex p1, Vertex p2);
    bool isPolygonViewable(Vertex p1, Vertex p2, Vertex p3);
};
#endif