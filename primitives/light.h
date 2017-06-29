#ifndef LIGHT_H
#define LIGHT_H

struct Light
{
    float r;
    float g;
    float b;
    float A;
    float B;
    std::vector<float> position;
    float rgb[3]; //convenience property used for shader. 

    Light(){};
    Light(float r, float g, float b, float A, float B, std::vector<float> position) 
        : r(r), g(g), b(b), A(A), B(B), position(position) 
        { //this is awkward but for purposes of project, will be sufficient
            rgb[0] = r;
            rgb[1] = g;
            rgb[2] = b;
        }
};
#endif