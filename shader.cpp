#include "shader.h"
#include <stack>
#include <math.h>
#include <iostream>

int LIGHTING_STYLE = PHONG;

Shader::Shader()
{
    ambient = Ambient(0, 0, 0);
    addSurface(1, 1, 1, .3, 8);
    numberOfLights = 0;
    normalAveraging = false;
}

void Shader::addLightSource(float r, float g, float b, float A, float B, std::vector<float> position)
{
    Light l = Light(r, g, b, A, B, position);
    lights[numberOfLights++] =  l;
}

void Shader::addSurface(float r, float g, float b, float ks, float alpha)
{
    Surface s = Surface(r, g, b, ks, alpha);
    surfaces.push_back(s);
    currentSurface = s;
}

void Shader::removeSurface()
{
    surfaces.pop_back();
    currentSurface = surfaces.back();
}

void Shader::setLightingStyle(int style)
{
    LIGHTING_STYLE = style;
}

void Shader::setAmbient(float r, float g, float b)
{
    ambient.setAmbient(r, g, b);
}

int Shader::getLightingStyle()
{
    return LIGHTING_STYLE;
}

Color Shader::flat(Vertex p1, Vertex p2, Vertex p3, Color centerColor, float q_x, float q_y, float q_z)
{
    std::vector<float> n(3);
    std::vector<float> q(3);
    q[0] = q_x;
    q[1] = q_y;
    q[2] = q_z;

    if (p1.normalDefined && p2.normalDefined && p3.normalDefined)
    {
        n[0] = (p1.normal.x + p2.normal.x + p3.normal.x)/3;
        n[1] = (p1.normal.y + p2.normal.y + p3.normal.y)/3;
        n[2] = (p1.normal.z + p2.normal.z + p3.normal.z)/3;
    }
    else
    {
        n = crossProduct(p1, p2, p3);
    }

    n = normalizeVector(n[0], n[1], n[2]);
   
    if (n[0] != n[0] || n[1] != n[1] || n[2] != n[2]) //check for nan 
        return Color(0, 0, 0);
    
    return lightingModel(centerColor, q, n);
}

Color Shader::gouraud(Vertex p1, Vertex p2, Vertex p3, Vertex pointToGet)
{
    std::vector<float> n(3);
    std::vector<float> q(3);
    q[0] = pointToGet.world_x;
    q[1] = pointToGet.world_y;
    q[2] = pointToGet.world_z;

    if (normalAveraging)
    {
        n[0] = (p1.normal.x + p2.normal.x + p3.normal.x)/3;
        n[1] = (p1.normal.y + p2.normal.y + p3.normal.y)/3;
        n[2] = (p1.normal.z + p2.normal.z + p3.normal.z)/3;
    }
    else if (pointToGet.normalDefined)
    {        
        n[0] = pointToGet.normal.x;
        n[1] = pointToGet.normal.y;
        n[2] = pointToGet.normal.z;
    }
    else
    {
        n = crossProduct(p1, p2, p3);
    }
    
    n = normalizeVector(n[0], n[1], n[2]);
    
    if (n[0] != n[0] || n[1] != n[1] || n[2] != n[2]) //check for nan 
        return Color(0, 0, 0);

    return lightingModel(pointToGet.color, q, n);
}

Normal Shader::getPhongNormal(Vertex p1, Vertex p2, Vertex p3, Vertex pointToGet)
{

    std::vector<float> n(3);

    if (normalAveraging)
    {
        n[0] = (p1.normal.x + p2.normal.x + p3.normal.x)/3;
        n[1] = (p1.normal.y + p2.normal.y + p3.normal.y)/3;
        n[2] = (p1.normal.z + p2.normal.z + p3.normal.z)/3;
    }
    else if (pointToGet.normalDefined)
    {        
        n[0] = pointToGet.normal.x;
        n[1] = pointToGet.normal.y;
        n[2] = pointToGet.normal.z;
    }
    else
    {
        n = crossProduct(p1, p2, p3);
    }

    return Normal(n[0], n[1], n[2]);
}

Color Shader::lightingModel(Color kd, std::vector<float> q, std::vector<float> N)
{
    float color[3]; //Final output RGB
    float ambientComponent[3]; //RGB
    float lightsSum[3] = {0, 0, 0}; //RGB
    float kdRGB[3] = {kd.r, kd.g, kd.b};

    float d, attenuation, specularComponent;

    std::vector<float> R(3);
    std::vector<float> L(3);
    std::vector<float> V(3);
    V = normalizeVector(-q[0], -q[1], -q[2]);

    // if color is unspecified, set to surface default
    if (kd.r == -1 || kd.g == -1 || kd.b == -1)
    {     
        kd.r = currentSurface.r;
        kd.g = currentSurface.g;
        kd.b = currentSurface.b;        
    }

    ambientComponent[0] = kd.r * ambient.r;
    ambientComponent[1] = kd.g * ambient.g;
    ambientComponent[2] = kd.b * ambient.b;

    for (int i = 0; i < numberOfLights; i++)
    {
        Light light = lights[i];
     
        d = sqrt(pow(q[0] - light.position[0], 2) + pow(q[1] - light.position[1], 2) + pow(q[2] - light.position[2], 2));
        L = normalizeVector(q[0] - light.position[0], q[1] - light.position[1], q[2] - light.position[2]);
        attenuation = 1 / (light.A + (light.B * d));
        float cross = dotProduct(N, L);
        R = normalizeVector((2 * cross * N[0]) - L[0], (2 * cross * N[1]) - L[1], (2 * cross * N[2]) - L[2]);
        R[0] =(2 * cross * N[0]) - L[0];
        R[1] = (2 * cross * N[1]) - L[1];
        R[2] = (2 * cross * N[2]) - L[2];

        specularComponent = currentSurface.ks * pow(dotProduct(V, R), currentSurface.alpha);

        for (int j = 0; j < 3; j++) //For each RGB
        {
            lightsSum[j] += (light.rgb[j] * attenuation) * (kdRGB[j] * dotProduct(N, L) + specularComponent);
        }
    }

    for (int j = 0; j < 3; j++) //For each RGB
    {
        color[j] = ambientComponent[j] + lightsSum[j];
        if (color[j] < 0)
            color[j] = 0;
        if (color[j] > 1)
            color[j] = 1;
    }
        
    return Color(color[0], color[1], color[2]);
}

float Shader::dotProduct(std::vector<float> a, std::vector<float> b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

std::vector<float> Shader::crossProduct(Vertex p1, Vertex p2, Vertex p3)
{
    std::vector<float> newVector(3);

    float x_a, y_a, z_a, x_b, y_b, z_b;

    x_a = p2.world_x - p1.world_x;
    y_a = p2.world_y - p1.world_y;
    z_a = p2.world_z - p1.world_z;

    x_b = p3.world_x - p1.world_x;
    y_b = p3.world_y - p1.world_y;
    z_b = p3.world_z - p1.world_z;

    newVector[0] = (y_a * z_b) - (y_b * z_a);
    newVector[1] = (z_a * x_b) - (z_b * x_a);
    newVector[2] = (x_a * y_b) - (x_b * y_a);

    return newVector;
}

std::vector<float> Shader::normalizeVector(float x, float y, float z)
{
    std::vector<float> newVector(3);
    float length = sqrt((x * x) + (y * y) + (z * z));

    newVector[0] = x/length;    
    newVector[1] = y/length;
    newVector[2] = z/length;

    return newVector;
}

Normal Shader::lerpNormal(Normal n1, Normal n2, float t)
{
    float old_x, old_y, old_z, new_x, new_y, new_z;
    std::vector<float> newVector(3);

    old_x =  n1.x * (1.0f - t);
    old_y =  n1.y * (1.0f - t);
    old_z =  n1.z * (1.0f - t);

    new_x =  n2.x * t;
    new_y =  n2.y * t;
    new_z =  n2.z * t;

    newVector[0] = old_x + new_x;
    newVector[1] = old_y + new_y;
    newVector[2] = old_z + new_z;

    return Normal(newVector[0], newVector[1], newVector[2]);
}