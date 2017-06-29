#ifndef SCANLINE_H
#define SCANLINE_H
#include "primitives/color.h"
#include "primitives/normal.h"
#include <vector>

struct Scanline
{
    Color color1;
    Color color2;
    float x1;
    float x2;
    float z1;
    float z2;
    float dx;
    float dz;
    Normal n1;
    Normal n2;
    std::vector<float> lerpPos_1;
    std::vector<float> lerpPos_2;

    Scanline(float x1, float z1, Color &color1, float x2, float z2, Color &color2, Normal n1, Normal n2, std::vector<float> lerpPos_1, std::vector<float> lerpPos_2);
};
#endif