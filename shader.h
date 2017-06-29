#ifndef SHADER_H
#define SHADER_H

#include <list>
#include <vector>
#include "primitives/light.h"
#include "primitives/vertex.h"
#include "primitives/surface.h"
#include "primitives/ambient.h"

#define PHONG 0
#define GOURAUD 1
#define FLAT 2

class Shader
{
public:
    Shader();
    void addLightSource(float r, float g, float b, float A, float B, std::vector<float> position);
    void addSurface(float r, float g, float b, float ks, float alpha);
    void setAmbient(float r, float g, float b);
    void removeSurface();
    void setLightingStyle(int style);
    int getLightingStyle();
    std::vector<float> normalizeVector(float x, float y, float z);
    Color flat(Vertex p1, Vertex p2, Vertex p3, Color centerColor, float q_x, float q_y, float q_z);
    Color gouraud(Vertex p1, Vertex p2, Vertex p3, Vertex pointToGet);
    Normal getPhongNormal(Vertex p1, Vertex p2, Vertex p3, Vertex pointToGet);
    Color lightingModel(Color kd, std::vector<float> q, std::vector<float> n);
    Normal lerpNormal(Normal n1, Normal n2, float t);
    bool normalAveraging;

private:
    Light lights[10]; // not great way but will do
    std::list<Surface> surfaces; //Only a list because mesh may have its own surface
    Ambient ambient;
    int numberOfLights;

    std::vector<float> crossProduct(Vertex p1, Vertex p2, Vertex p3);
    float dotProduct(std::vector<float> a, std::vector<float> b);
    Surface currentSurface;
};
#endif