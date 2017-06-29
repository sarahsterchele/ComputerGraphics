#include <stdlib.h> 
#include <math.h> 
#include <algorithm>
#include <iostream>
#include "renderer.h"

Renderer::Renderer(Drawable *drawable, Z_Buffer *zbuffer, Shader *shader) : drawable(drawable), zbuffer(zbuffer), shader(shader) {}

void Renderer::draw_Mesh(std::vector<std::vector<Vertex> > mesh, int rows, int cols, bool filledIn)
{
    for (int i = 0; i < rows - 1; i++)
    {
        for (int j = 0; j < cols - 1; j++)
        {
            draw_Polygon(mesh[i][j], mesh[i][j + 1], mesh[i + 1][j + 1], filledIn); 
            draw_Polygon(mesh[i][j], mesh[i + 1][j + 1], mesh[i + 1][j], filledIn);
        }
    }
}

void Renderer::draw_Line(Vertex p1, Vertex p2)
{
    if (!isLineViewable(p1, p2))
        return;

    Vertex P1 = p1;
    Vertex P2 = p2;
    float dx, dy, m, b, t, t_step;
    int y_round;

    dx = P2.x - P1.x;
    dy = P2.y - P1.y;

    if (dx == 0 || dy == 0)
    {
        if (dx == 0)  // Vertical line
        {       
            if (P1.y > P2.y)
            {
                std::swap(P1.y, P2.y);
                std::swap(P1.z, P2.z);
                std::swap(P1.color, P2.color);
            }

            dy = P2.y - P1.y;
            t = 0.0f;
            t_step = 1 / dy;

            for (int y = P1.y; y < P2.y; y++)
            {

               if (isVertexViewable(P1.x, y, lerp(P1.z, P2.z, t)))
               {
                    drawable->setPixel(P1.x, y, lerpColor(P1.color, P2.color, t).color);
               }

               t += t_step;
            }
        }
        else if (dy == 0) // Horizontal line
        {
            if (P1.x > P2.x)
            {
                std::swap(P1.x, P2.x);
                std::swap(P1.z, P2.z);
                std::swap(P1.color, P2.color);
            }

            dx = P2.x - P1.x;
            t = 0.0f;
            t_step = 1/dx;

            for (int x = P1.x; x < P2.x; x++)
            {
                if (isVertexViewable(x, P1.y, lerp(P1.z, P2.z, t)))
                {
                    drawable->setPixel(x, P1.y, lerpColor(P1.color, P2.color, t).color);
                }

                t += t_step;
            }
        }
        return;
    }

    bool slope = (abs(P2.y - P1.y) > abs(P2.x - P1.x));
    if (slope)
    {
        std::swap(P1.x, P1.y);
        std::swap(P2.x, P2.y);
    }
    if (P1.x > P2.x)
    {
        std::swap(P1.x, P2.x);
        std::swap(P1.y, P2.y);
        std::swap(P1.color, P2.color);
        std::swap(P1.z, P2.z);
    }

    m = ((float) P2.y - (float) P1.y)/((float) P2.x - (float) P1.x);
    b = P2.y - m*(P2.x);

    dx = P2.x - P1.x;
    t = 0.0f;
    t_step = 1/dx;

    for (int x = P1.x; x < P2.x; x++)
    {
        y_round = round(m*x + b);

        if (slope)
        {
            if (isVertexViewable(y_round, x, lerp(P1.z, P2.z, t)))
            {
                drawable->setPixel(y_round, x, lerpColor(P1.color, P2.color, t).color);
            }
        }
        else
        {
            if (isVertexViewable(x, y_round, lerp(P1.z, P2.z, t)))
            {
                drawable->setPixel(x, y_round, lerpColor(P1.color, P2.color, t).color);
            }
        }

        t += t_step;
     }
}


void Renderer::draw_Polygon(Vertex p1, Vertex p2, Vertex p3, bool isFilled)
{
    if (!isFilled)
    {
        draw_Line(p1, p2);
        draw_Line(p1, p3);
        draw_Line(p2, p3);
       return;
    } 

    if (shader->getLightingStyle() == GOURAUD)
    {
        p1.color = shader->gouraud(p1, p2, p3, p1);
        p2.color = shader->gouraud(p1, p2, p3, p2);
        p3.color = shader->gouraud(p1, p2, p3, p3);
    }
    else if (shader->getLightingStyle() == PHONG)
    {
        p1.normal = shader->getPhongNormal(p1, p2, p3, p1);
        p2.normal = shader->getPhongNormal(p1, p2, p3, p2);
        p3.normal = shader->getPhongNormal(p1, p2, p3, p3);
    }

    PolygonEdge edges[3] = 
        {   
            PolygonEdge(p1, p2),
            PolygonEdge(p2, p3),   
            PolygonEdge(p3, p1) 
        };

    float maxLength = 0.0f;
    int longestLineIndex = 0;

    // find edge with the greatest length in the y axis
    for(int i = 0; i < 3; i++) 
    {
        float length = edges[i].P2.y - edges[i].P1.y;
        if (length > maxLength) 
        {
            maxLength = length;
            longestLineIndex = i;
        }
    }

    int shortEdge1 = (longestLineIndex + 1) % 3;
    int shortEdge2 = (longestLineIndex + 2) % 3;

    Color shaderColor;
    if (shader->getLightingStyle() == FLAT)
    {
        Color e1 = lerpColor(edges[0].P1.color, edges[0].P2.color, .5);
        Color e2 = lerpColor(edges[1].P1.color, edges[1].P2.color, .5);
        Color e3 = lerpColor(edges[2].P1.color, edges[2].P2.color, .5);
        Color mp = lerpColor(e1, e2, .5);
        Color midpointcolor = lerpColor(mp, e3, .5);

        float x = lerp(lerp(lerp(edges[0].P1.world_x, edges[0].P2.world_x, .5), lerp(edges[1].P1.world_x, edges[1].P2.world_x, .5), .5), lerp(edges[2].P1.world_x, edges[2].P2.world_x, .5), .5);
        float y = lerp(lerp(lerp(edges[0].P1.world_y, edges[0].P2.world_y, .5), lerp(edges[1].P1.world_y, edges[1].P2.world_y, .5), .5), lerp(edges[2].P1.world_y, edges[2].P2.world_y, .5), .5);
        float z = lerp(lerp(lerp(edges[0].P1.world_z, edges[0].P2.world_z, .5), lerp(edges[1].P1.world_z, edges[1].P2.world_z, .5), .5), lerp(edges[2].P1.world_z, edges[2].P2.world_z, .5), .5);

        shaderColor = shader->flat(p1, p2, p3, midpointcolor, x, y, z); 
    }
    
    drawScanlinesBetweenEdges(edges[longestLineIndex], edges[shortEdge1], shaderColor);
    drawScanlinesBetweenEdges(edges[longestLineIndex], edges[shortEdge2], shaderColor); 
}

void Renderer::drawScanlinesBetweenEdges(PolygonEdge &e1, PolygonEdge &e2, Color shaderColor)
{
    if(e1.dy == 0.0f)
        return;

    if(e2.dy == 0.0f)
        return;

    float factor_y1 = (float) (e2.P1.y - e1.P1.y) / e1.dy;
    float factorStep_y1 = 1.0f / e1.dy;

    float factor_y2 = 0.0f;
    float factorStep_y2 = 1.0f / e2.dy;

    for (int y = e2.P1.y; y < e2.P2.y; y++) 
    {
        Color color1, color2;
        Normal n1, n2;
        std::vector<float> lerpPos_1(3);
        std::vector<float> lerpPos_2(3);

        if (shader->getLightingStyle() == FLAT)
        {
            color1 = shaderColor;
            color2 = shaderColor;
        }
        else
        {
            if (shader->getLightingStyle() == PHONG)
            {
                n1 = shader->lerpNormal(e1.P1.normal, e1.P2.normal, factor_y1);
                lerpPos_1 = lerpVector(e1.P1.world_x, e1.P1.world_y, e1.P1.world_z, e1.P2.world_x, e1.P2.world_y, e1.P2.world_z, factor_y1);

                n2 = shader->lerpNormal(e2.P1.normal, e2.P2.normal, factor_y2);
                lerpPos_2 = lerpVector(e2.P1.world_x, e2.P1.world_y, e2.P1.world_z, e2.P2.world_x, e2.P2.world_y, e2.P2.world_z, factor_y2);
            }

            color1 = lerpColor(e1.P1.color, e1.P2.color, factor_y1);
            color2 = lerpColor(e2.P1.color, e2.P2.color, factor_y2);
        }

        Scanline scanLine
        (
            e1.P1.x + (int) (e1.dx * factor_y1), 
            lerp(e1.P1.z, e1.P2.z, factor_y1),
            color1,                
            e2.P1.x + (int) (e2.dx * factor_y2),
            lerp(e2.P1.z, e2.P2.z, factor_y2),
            color2, 
            n1, 
            n2, 
            lerpPos_1,
            lerpPos_2
        );

        draw_Scanline(scanLine, y);

        factor_y1 += factorStep_y1;
        factor_y2 += factorStep_y2;
    }
}

void Renderer::draw_Scanline(Scanline &scanLine, float y)
{
    if(scanLine.dx == 0.0f)
        return;
    
    int scandx = scanLine.dx;
    
    float t = 0.0f;
    float t_step = 1.0f / (float) scandx;

    for (int x = scanLine.x1; x < scanLine.x2; x++) 
    {
        if (isVertexViewable(x, y, lerp(scanLine.z1, scanLine.z2, t)))
        {
            Color newColor;
            if (shader->getLightingStyle() == PHONG)
            {
                Color color = lerpColor(scanLine.color1, scanLine.color2, t);
                std::vector<float> q = lerpVector(scanLine.lerpPos_1, scanLine.lerpPos_2, t);
                Normal lerpNormal = shader->lerpNormal(scanLine.n1, scanLine.n2, t);
                std::vector<float> N(3);
                N = shader->normalizeVector(lerpNormal.x, lerpNormal.y, lerpNormal.z);

                if (N[0] != N[0] || N[1] != N[1] || N[2] != N[2]) //check for nan
                    newColor = Color(0, 0, 0);
                else   
                    newColor = shader->lightingModel(color, q, N);
            }
            else
            {
                newColor = lerpColor(scanLine.color1, scanLine.color2, t);
            }

            drawable->setPixel(x, y, newColor.color);
        }
        t += t_step;
    }
}

Color Renderer::lerpColor(Color &color1, Color &color2, float t)
{
    float old_R, old_B, old_G, new_R, new_B, new_G, R, G, B;

    old_B = (float) color1.b * (1.0f - t);
    old_G = (float) color1.g * (1.0f - t);
    old_R = (float) color1.r * (1.0f - t);

    new_B = (float) color2.b * t;
    new_G = (float) color2.g * t;
    new_R = (float) color2.r * t;

    R = old_R + new_R;
    G = old_G + new_G;
    B = old_B + new_B;

    return Color(R, G, B);
}

std::vector<float> Renderer::lerpVector(float x1, float y1, float z1, float x2, float y2, float z2, float t)
{
    std::vector<float> newVector(3);

    float old_X, old_Y, old_Z, new_X, new_Y, new_Z;

    old_X = x1 * (1.0f - t);
    old_Y = y1 * (1.0f - t);
    old_Z = z1 * (1.0f - t);

    new_X = x2 * t;
    new_Y = y2 * t;
    new_Z = z2 * t;

    newVector[0] = old_X + new_X;
    newVector[1] = old_Y + new_Y;
    newVector[2] = old_Z + new_Z;

    return newVector;
}

std::vector<float> Renderer::lerpVector(std::vector<float> p1, std::vector<float> p2, float t)
{
     std::vector<float> newVector(3);

    float old_X, old_Y, old_Z, new_X, new_Y, new_Z;

    old_X = p1[0] * (1.0f - t); // X
    old_Y = p1[1] * (1.0f - t); // Y
    old_Z = p1[2] * (1.0f - t); // Z

    new_X = p2[0] * t;
    new_Y = p2[1] * t;
    new_Z = p2[2] * t;

    newVector[0] = old_X + new_X;
    newVector[1] = old_Y + new_Y;
    newVector[2] = old_Z + new_Z;

    return newVector;
}

float Renderer::lerp(float p1, float p2, float t)
{
    return (((1.0f - t) * p1) + (t * p2));
}

bool Renderer::isVertexViewable(int x, int y, float z)
{
    if (z > 2.0f || z < 1.0f)
        return false;
    if (x >= 700.0f || x <  50.0f)
        return false;
    if (y >= 700.0f || y < 50.0f)
        return false;

    return (zbuffer->isVisible(x, y, z));
}

bool Renderer::isLineViewable(Vertex p1, Vertex p2)
{
    if (p1.z > 2.0f && p2.z > 2.0f)
        return false;

    if (p1.z < 1.0f && p2.z < 1.0f)
        return false;

    //Is x axis viewable?
    if (p1.x >= 700.0f && p2.x >= 700.0f)
        return false;
    if (p1.x < 50.0f && p2.x < 50.0f)
        return false;
    
    //Is y axis viewable?
    if (p1.y >= 700.0f && p2.y >= 700.0f)
        return false;
    if (p1.y < 50.0f && p2.y < 50.0f)
        return false;

    return true;
}

