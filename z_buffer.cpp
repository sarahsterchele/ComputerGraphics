#include "z_buffer.h"
#include <iostream>
#include <limits>

Z_Buffer::Z_Buffer()
{
    depthBuffer.resize(800, std::vector<float>(800, std::numeric_limits<float>::infinity()));
}

bool Z_Buffer::isVisible(int x, int y, float z)
{
    float currentZ = depthBuffer[x][y];
    if (currentZ < z)
    {
        return false;
    }

    update(x, y, z);
    return true;
}

float Z_Buffer::getDepth(int x, int y)
{
    return depthBuffer[x][y];
}

void Z_Buffer::update(int x, int y, float z)
{
    depthBuffer[x][y] = z;
}
