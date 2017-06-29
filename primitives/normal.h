#ifndef NORMAL_H
#define NORMAL_H

struct Normal
{
    float x;
    float y;
    float z;

    Normal(){}
    Normal(float x, float y, float z) : x(x), y(y), z(z){}
};
#endif