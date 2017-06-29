#ifndef SURFACE_H
#define SURFACE_H

struct Surface
{
    float r;
    float g;
    float b;
    float ks;
    float alpha;

    Surface(){};
    Surface(float r, float g, float b, float ks, float alpha) : r(r), g(g), b(b), ks(ks), alpha(alpha){};
};
#endif