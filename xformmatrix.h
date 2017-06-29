#ifndef XFORM_MATRIX_H
#define XFORM_MATRIX_H

#include "renderer.h"
#include <stack>

class XformMatrix
{
public:
    std::vector<std::vector<float> > *currentMatrix; //current transform matrix
    std::vector<std::vector<float> > cameraToWorldCTM; 
    std::vector<std::vector<float> > objectToWorldCTM;
    std::vector<std::vector<float> > inverseCTM; 
    std::vector<std::vector<float> > perspectiveMatrix; 

    XformMatrix();
    void scale(float x, float y, float z);
    void translate(float x, float y, float z);
    void rotateX(float degrees);
    void rotateY(float degrees);
    void rotateZ(float degrees);
    void addCamera(float xlow, float ylow, float xhigh, float yhigh, float hither, float yon);
    void pushCTM();
    void popCTM();
    Vertex getTransformedVertex(float x, float y, float z, float r, float g, float b, bool hasNormal, float nx, float ny, float nz);
    std::vector<float> getWorldPosition(float x, float y, float z);

private:
    std::stack<std::vector <std::vector<float> > > matrixStack;
    std::vector <std::vector<float> > multMatrix(std::vector <std::vector<float> > A, std::vector <std::vector<float> > B);
    std::vector <std::vector<float> > inverseMatrix(std::vector <std::vector<float> > m);

    float getCos(float degrees);
    float getSin(float degrees);
    float convertToDegrees(float radians);
    float convertToRadians(float degrees);
    void printMatrix(std::vector <std::vector<float> > matrix);
};
#endif