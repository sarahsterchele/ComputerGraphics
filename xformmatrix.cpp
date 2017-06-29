#include "xformmatrix.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> 

#define PI 3.14159265358979323846264338327

float scaleX = 1;
float scaleY = 1;
float centerX = 0;
float centerY = 0;

XformMatrix::XformMatrix()
{
    objectToWorldCTM.resize(4, std::vector<float>(4));
    cameraToWorldCTM.resize(4, std::vector<float>(4));
    perspectiveMatrix.resize(4, std::vector<float>(4));
    inverseCTM.resize(4, std::vector<float>(4));

    objectToWorldCTM[0][0] = 1;
    objectToWorldCTM[0][1] = 0;
    objectToWorldCTM[0][2] = 0;
    objectToWorldCTM[0][3] = 0;

    objectToWorldCTM[1][0] = 0;
    objectToWorldCTM[1][1] = 1;
    objectToWorldCTM[1][2] = 0;
    objectToWorldCTM[1][3] = 0;

    objectToWorldCTM[2][0] = 0;
    objectToWorldCTM[2][1] = 0;
    objectToWorldCTM[2][2] = 1;
    objectToWorldCTM[2][3] = 0;
    
    objectToWorldCTM[3][0] = 0;
    objectToWorldCTM[3][1] = 0;
    objectToWorldCTM[3][2] = 0;
    objectToWorldCTM[3][3] = 1;

    cameraToWorldCTM[0][0] = 1;
    cameraToWorldCTM[0][1] = 0;
    cameraToWorldCTM[0][2] = 0;
    cameraToWorldCTM[0][3] = 0;

    cameraToWorldCTM[1][0] = 0;
    cameraToWorldCTM[1][1] = 1;
    cameraToWorldCTM[1][2] = 0;
    cameraToWorldCTM[1][3] = 0;

    cameraToWorldCTM[2][0] = 0;
    cameraToWorldCTM[2][1] = 0;
    cameraToWorldCTM[2][2] = 1;
    cameraToWorldCTM[2][3] = 0;
    
    cameraToWorldCTM[3][0] = 0;
    cameraToWorldCTM[3][1] = 0;
    cameraToWorldCTM[3][2] = 0;
    cameraToWorldCTM[3][3] = 1;

    currentMatrix = &objectToWorldCTM;
}

void XformMatrix::scale(float x, float y, float z)
{
    std::vector <std::vector<float> > scale(4, std::vector<float>(4));
    {   
        scale[0][0] = x;
        scale[0][1] = 0;
        scale[0][2] = 0;
        scale[0][3] = 0;

        scale[1][0] = 0;
        scale[1][1] = y;
        scale[1][2] = 0;
        scale[1][3] = 0;

        scale[2][0] = 0;
        scale[2][1] = 0;
        scale[2][2] = z;
        scale[2][3] = 0;
        
        scale[3][0] = 0;
        scale[3][1] = 0;
        scale[3][2] = 0;
        scale[3][3] = 1;
    }

    *currentMatrix = multMatrix(*currentMatrix, scale);
}

void XformMatrix::translate(float x, float y, float z)
{
    std::vector <std::vector<float> > xlate(4, std::vector<float>(4));
    {
        xlate[0][0] = 1;
        xlate[0][1] = 0;
        xlate[0][2] = 0;
        xlate[0][3] = x;

        xlate[1][0] = 0;
        xlate[1][1] = 1;
        xlate[1][2] = 0;
        xlate[1][3] = -y;

        xlate[2][0] = 0;
        xlate[2][1] = 0;
        xlate[2][2] = 1;
        xlate[2][3] = z;
        
        xlate[3][0] = 0;
        xlate[3][1] = 0;
        xlate[3][2] = 0;
        xlate[3][3] = 1;
    }

    *currentMatrix = multMatrix(*currentMatrix, xlate);
}

void XformMatrix::rotateX(float degrees)
{
    std::vector <std::vector<float> > rotate(4, std::vector<float>(4));
    {
        rotate[0][0] = 1;
        rotate[0][1] = 0;
        rotate[0][2] = 0;
        rotate[0][3] = 0;
    
        rotate[1][0] = 0;
        rotate[1][1] = getCos(degrees);
        rotate[1][2] = (-1) * getSin(degrees);
        rotate[1][3] = 0;
    
        rotate[2][0] = 0;
        rotate[2][1] = getSin(degrees);
        rotate[2][2] = getCos(degrees);
        rotate[2][3] = 0;
        
        rotate[3][0] = 0;
        rotate[3][1] = 0;
        rotate[3][2] = 0;
        rotate[3][3] = 1;
    }
    *currentMatrix = multMatrix(*currentMatrix, rotate);
 }

void XformMatrix::rotateY(float degrees)
{
    std::vector <std::vector<float> > rotate(4, std::vector<float>(4));
    {   
        rotate[0][0] = getCos(degrees);
        rotate[0][1] = 0;
        rotate[0][2] = (-1) * getSin(degrees);
        rotate[0][3] = 0;

        rotate[1][0] = 0;
        rotate[1][1] = 1;
        rotate[1][2] = 0;
        rotate[1][3] = 0;

        rotate[2][0] = getSin(degrees);
        rotate[2][1] = 0;
        rotate[2][2] = getCos(degrees);
        rotate[2][3] = 0;
        
        rotate[3][0] = 0;
        rotate[3][1] = 0;
        rotate[3][2] = 0;
        rotate[3][3] = 1;
    }
    *currentMatrix = multMatrix(*currentMatrix, rotate);
}

void XformMatrix::rotateZ(float degrees)
{
    std::vector <std::vector<float> > rotate(4, std::vector<float>(4));
    {   
        rotate[0][0] = getCos(degrees);
        rotate[0][1] = (-1) * getSin(degrees);
        rotate[0][2] = 0;
        rotate[0][3] = 0;

        rotate[1][0] = getSin(degrees);
        rotate[1][1] = getCos(degrees);
        rotate[1][2] = 0;
        rotate[1][3] = 0;

        rotate[2][0] = 0;
        rotate[2][1] = 0;
        rotate[2][2] = 1;
        rotate[2][3] = 0;
        
        rotate[3][0] = 0;
        rotate[3][1] = 0;
        rotate[3][2] = 0;
        rotate[3][3] = 1;
    }
    *currentMatrix = multMatrix(*currentMatrix, rotate);
}

void XformMatrix::addCamera(float xlow, float ylow, float xhigh, float yhigh, float near, float far)
{
    inverseCTM = inverseMatrix(objectToWorldCTM);

    currentMatrix = &cameraToWorldCTM;

    centerX = (xlow + xhigh) / 2.0f;
    centerY = (ylow + yhigh) / 2.0f;
    scaleX = 200.0f / abs(xhigh - xlow);
    scaleY = 200.0f / abs(yhigh - ylow);

    float a = 2.0f - ( near / (near - far));
    float b = (far * near) / (near - far);

    perspectiveMatrix[0][0] = 1;
    perspectiveMatrix[0][1] = 0;
    perspectiveMatrix[0][2] = 0;
    perspectiveMatrix[0][3] = 0;

    perspectiveMatrix[1][0] = 0;
    perspectiveMatrix[1][1] = 1;
    perspectiveMatrix[1][2] = 0;
    perspectiveMatrix[1][3] = 0;

    perspectiveMatrix[2][0] = 0;
    perspectiveMatrix[2][1] = 0;
    perspectiveMatrix[2][2] = a;
    perspectiveMatrix[2][3] = b;

    perspectiveMatrix[3][0] = 0;
    perspectiveMatrix[3][1] = 0;
    perspectiveMatrix[3][2] = 1;
    perspectiveMatrix[3][3] = 0;
}

Vertex XformMatrix::getTransformedVertex(float x, float y, float z, float r, float g, float b, bool hasNormal, float nx, float ny, float nz)
{
    std::vector <std::vector<float> > newMatrix(4, std::vector<float>(4));

    newMatrix = multMatrix(perspectiveMatrix, multMatrix(cameraToWorldCTM, inverseCTM));

    float X, Y, Z, W;

    W = (newMatrix[3][0]*x + newMatrix[3][1]*(-y) + newMatrix[3][2]*z + newMatrix[3][3]);

    X = (newMatrix[0][0]*x + newMatrix[0][1]*(-y) + newMatrix[0][2]*z + newMatrix[0][3])/W;
    Y = (newMatrix[1][0]*x + newMatrix[1][1]*(-y) + newMatrix[1][2]*z + newMatrix[1][3])/W;
    Z = (newMatrix[2][0]*x + newMatrix[2][1]*(-y) + newMatrix[2][2]*z + newMatrix[2][3])/W;

    std::vector<float> world = getWorldPosition(x, y, z);

    return Vertex((X * 3.25 * scaleX) + centerX + 375, (Y * 3.25 * scaleY) + 375 + centerY, Z, world[0], world[1], world[2], Color(r, g, b), hasNormal, Normal(nx, ny, nz));
}

std::vector<float> XformMatrix::getWorldPosition(float x, float y, float z)
{
    std::vector <std::vector<float> > newMatrix(4, std::vector<float>(4));
    std::vector <float> position(3);
    newMatrix = multMatrix(perspectiveMatrix, multMatrix(cameraToWorldCTM, inverseCTM));

    float W;

    W = (newMatrix[3][0]*x + newMatrix[3][1]*(y) + newMatrix[3][2]*z + newMatrix[3][3]);
    position[0] = (newMatrix[0][0]*x + newMatrix[0][1]*(y) + newMatrix[0][2]*z + newMatrix[0][3])/W;
    position[1] = (newMatrix[1][0]*x + newMatrix[1][1]*(y) + newMatrix[1][2]*z + newMatrix[1][3])/W;
    position[2] = (newMatrix[2][0]*x + newMatrix[2][1]*(y) + newMatrix[2][2]*z + newMatrix[2][3])/W;

    return position;
}

/* Helper Functions */

std::vector <std::vector<float> > XformMatrix::multMatrix(std::vector <std::vector<float> > A, std::vector <std::vector<float> > B)
{
    std::vector <std::vector<float> > newMatrix(4, std::vector<float>(4, 0));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                newMatrix[i][j] += A[i][k] * B[k][j];
            }
        }       
    }
    return newMatrix;
}

void XformMatrix::printMatrix(std::vector <std::vector<float> > matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << matrix[i][j] << " " ;
        }
        std::cout << std::endl;
    }
}

float XformMatrix::getCos(float degrees)
{
    switch((int) degrees)
    {
    case 0:
        return 1;
    case 60:
        return .5;
    case -60:
        return -.5;
    case 90:
        return 0;       
    case -90:
        return 0;
    case 180:
        return -1;
    case -180:
        return 1;
    case 270:
        return 0;
    case -270:
        return 0;
    case 360:
        return 1;
    case -360:
        return -1;
    }

    return cos(convertToRadians(degrees));
}

float XformMatrix::getSin(float degrees)
{
    switch((int) degrees)
    {
    case 0:
        return 0;
    case 30:
        return .5;
    case -30:
        return -.5;
    case 90:
        return 1;
    case -90:
        return -1;
    case 180:
        return 0;
    case -180:
        return 0;
    case 270:
        return -1;
    case -270:
        return 1;
    case 360:
        return 0;
    case -360:
        return 0;
    }

    return sin(convertToRadians(degrees));
}

float XformMatrix::convertToRadians(float degrees)
{
    return degrees * PI / 180.0f;
}

float XformMatrix::convertToDegrees(float radians)
{
    return radians * 180.0f / PI;
}

void XformMatrix::pushCTM()
{
    matrixStack.push(*currentMatrix);
}

void XformMatrix::popCTM()
{
    *currentMatrix = matrixStack.top();
    matrixStack.pop();
}

std::vector <std::vector<float> > XformMatrix::inverseMatrix(std::vector <std::vector<float> > m)
{
    std::vector <std::vector<float> > inv(4, std::vector<float>(4, 0));
    float det;

    inv[0][0] = m[1][1]  * m[2][2] * m[3][3] - 
             m[1][1]  * m[2][3] * m[3][2] - 
             m[2][1]  * m[1][2]  * m[3][3] + 
             m[2][1]  * m[1][3]  * m[3][2] +
             m[3][1] * m[1][2]  * m[2][3] - 
             m[3][1] * m[1][3]  * m[2][2];

    inv[1][0] = -m[1][0]  * m[2][2] * m[3][3] + 
              m[1][0]  * m[2][3] * m[3][2] + 
              m[2][0]  * m[1][2]  * m[3][3] - 
              m[2][0]  * m[1][3]  * m[3][2] - 
              m[3][0] * m[1][2]  * m[2][3] + 
              m[3][0] * m[1][3]  * m[2][2];

    inv[2][0] = m[1][0]  * m[2][1] * m[3][3] - 
             m[1][0]  * m[2][3] * m[3][1] - 
             m[2][0]  * m[1][1] * m[3][3] + 
             m[2][0]  * m[1][3] * m[3][1] + 
             m[3][0] * m[1][1] * m[2][3] - 
             m[3][0] * m[1][3] * m[2][1];

    inv[3][0] = -m[1][0]  * m[2][1] * m[3][2] + 
               m[1][0]  * m[2][2] * m[3][1] +
               m[2][0]  * m[1][1] * m[3][2] - 
               m[2][0]  * m[1][2] * m[3][1] - 
               m[3][0] * m[1][1] * m[2][2] + 
               m[3][0] * m[1][2] * m[2][1];

    inv[0][1] = -m[0][1]  * m[2][2] * m[3][3] + 
              m[0][1]  * m[2][3] * m[3][2] + 
              m[2][1]  * m[0][2] * m[3][3] - 
              m[2][1]  * m[0][3] * m[3][2] - 
              m[3][1] * m[0][2] * m[2][3] + 
              m[3][1] * m[0][3] * m[2][2];

    inv[1][1] = m[0][0]  * m[2][2] * m[3][3] - 
             m[0][0]  * m[2][3] * m[3][2] - 
             m[2][0]  * m[0][2] * m[3][3] + 
             m[2][0]  * m[0][3] * m[3][2] + 
             m[3][0] * m[0][2] * m[2][3] - 
             m[3][0] * m[0][3] * m[2][2];

    inv[2][1] = -m[0][0]  * m[2][1] * m[3][3] + 
              m[0][0]  * m[2][3] * m[3][1] + 
              m[2][0]  * m[0][1] * m[3][3] - 
              m[2][0]  * m[0][3] * m[3][1] - 
              m[3][0] * m[0][1] * m[2][3] + 
              m[3][0] * m[0][3] * m[2][1];

    inv[3][1] = m[0][0]  * m[2][1] * m[3][2] - 
              m[0][0]  * m[2][2] * m[3][1] - 
              m[2][0]  * m[0][1] * m[3][2] + 
              m[2][0]  * m[0][2] * m[3][1] + 
              m[3][0] * m[0][1] * m[2][2] - 
              m[3][0] * m[0][2] * m[2][1];

    inv[0][2] = m[0][1]  * m[1][2] * m[3][3] - 
             m[0][1]  * m[1][3] * m[3][2] - 
             m[1][1]  * m[0][2] * m[3][3] + 
             m[1][1]  * m[0][3] * m[3][2] + 
             m[3][1] * m[0][2] * m[1][3] - 
             m[3][1] * m[0][3] * m[1][2];

    inv[1][2] = -m[0][0]  * m[1][2] * m[3][3] + 
              m[0][0]  * m[1][3] * m[3][2] + 
              m[1][0]  * m[0][2] * m[3][3] - 
              m[1][0]  * m[0][3] * m[3][2] - 
              m[3][0] * m[0][2] * m[1][3] + 
              m[3][0] * m[0][3] * m[1][2];

    inv[2][2] = m[0][0]  * m[1][1] * m[3][3] - 
              m[0][0]  * m[1][3] * m[3][1] - 
              m[1][0]  * m[0][1] * m[3][3] + 
              m[1][0]  * m[0][3] * m[3][1] + 
              m[3][0] * m[0][1] * m[1][3] - 
              m[3][0] * m[0][3] * m[1][1];

    inv[3][2] = -m[0][0]  * m[1][1] * m[3][2] + 
               m[0][0]  * m[1][2] * m[3][1] + 
               m[1][0]  * m[0][1] * m[3][2] - 
               m[1][0]  * m[0][2] * m[3][1] - 
               m[3][0] * m[0][1] * m[1][2] + 
               m[3][0] * m[0][2] * m[1][1];

    inv[0][3] = -m[0][1] * m[1][2] * m[2][3] + 
              m[0][1] * m[1][3] * m[2][2] + 
              m[1][1] * m[0][2] * m[2][3] - 
              m[1][1] * m[0][3] * m[2][2] - 
              m[2][1] * m[0][2] * m[1][3] + 
              m[2][1] * m[0][3] * m[1][2];

    inv[1][3] = m[0][0] * m[1][2] * m[2][3] - 
             m[0][0] * m[1][3] * m[2][2] - 
             m[1][0] * m[0][2] * m[2][3] + 
             m[1][0] * m[0][3] * m[2][2] + 
             m[2][0] * m[0][2] * m[1][3] - 
             m[2][0] * m[0][3] * m[1][2];

    inv[2][3] = -m[0][0] * m[1][1] * m[2][3] + 
               m[0][0] * m[1][3] * m[2][1] + 
               m[1][0] * m[0][1] * m[2][3] - 
               m[1][0] * m[0][3] * m[2][1] - 
               m[2][0] * m[0][1] * m[1][3] + 
               m[2][0] * m[0][3] * m[1][1];

    inv[3][3] = m[0][0] * m[1][1] * m[2][2] - 
              m[0][0] * m[1][2] * m[2][1] - 
              m[1][0] * m[0][1] * m[2][2] + 
              m[1][0] * m[0][2] * m[2][1] + 
              m[2][0] * m[0][1] * m[1][2] - 
              m[2][0] * m[0][2] * m[1][1];

    det = m[0][0] * inv[0][0] + m[0][1] * inv[1][0] + m[0][2] * inv[2][0] + m[0][3] * inv[3][0];

    det = 1.0f / det;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            inv[i][j] = inv[i][j] * det;

        return inv;
}