#include "color.h"
#include <math.h>

unsigned int Color::convertColor(float r, float g, float b)
{
    float R, G, B;
    unsigned int mod_color;

    R = r * 255.0f;
    G = g * 255.0f;
    B = b * 255.0f;

    mod_color = 0xFF << 24;
    mod_color = mod_color | (unsigned int) round(R) << 16;
    mod_color = mod_color | (unsigned int) round(G) << 8;
    mod_color = mod_color | (unsigned int) round(B);

    return mod_color;
}