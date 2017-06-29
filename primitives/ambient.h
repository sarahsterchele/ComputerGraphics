#ifndef AMBIENT_H
#define AMBIENT_H

struct Ambient
{
    float r;
    float g;
    float b;

    Ambient(){};
    Ambient(float r, float g, float b) : r(r), g(g), b(b) {}
    void setAmbient(float r, float g, float b) { this->r = r; this->g = g; this->b = b;}
};
#endif