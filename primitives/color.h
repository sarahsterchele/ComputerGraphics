#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    float r;
    float g;
    float b;
    unsigned int color;

    Color(){}
    Color(float r, float g, float b) : r(r), g(g), b(b), color(convertColor(r, g, b)){}

    unsigned int convertColor(float r, float g, float b);
};
#endif