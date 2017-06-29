#include "scanline.h"

Scanline::Scanline(float x1, float z1, Color &color1, float x2, float z2, Color &color2, Normal n1, Normal n2, std::vector<float> lerpPos_1, std::vector<float> lerpPos_2)
{
    if(x1 < x2) 
    {
        this->x1 = x1;
        this->x2 = x2;
        this->color1 = color1;
        this->color2 = color2;
        this->z1 = z1;
        this->z2 = z2;
        this->n1 = n1;
        this->n2 = n2;
        this->lerpPos_1 = lerpPos_1;
        this->lerpPos_2 = lerpPos_2;
    } 
    else 
    {
        this->x1 = x2;
        this->x2 = x1;
        this->color2 = color1;
        this->color1 = color2;
        this->z1 = z2;
        this->z2 = z1;
        this->n1 = n2;
        this->n2 = n1;
        this->lerpPos_1 = lerpPos_2;
        this->lerpPos_2 = lerpPos_1;
    }

    dx = this->x2 - this->x1;
    dz = this->z2 - this->z1;
}