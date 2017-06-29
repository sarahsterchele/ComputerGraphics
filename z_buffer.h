#ifndef Z_BUFFER_H
#define Z_BUFFER_H

#include <vector>

class Z_Buffer
{
public:
    Z_Buffer();
    bool isVisible(int x, int y, float z);
    float getDepth(int x, int y);
private:
    std::vector <std::vector<float> > depthBuffer;
    void update(int x, int y, float value);

};
#endif