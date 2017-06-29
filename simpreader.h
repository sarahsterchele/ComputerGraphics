#ifndef SIMP_READER_H
#define SIMP_READER_H

#include "xformmatrix.h"

class SimpReader
{
public:
    SimpReader(Drawable *drawable);
    void readSimpFile(const char* filename);

private:
    Drawable *drawable;
    Z_Buffer *zbuffer;
    Renderer *renderer;
    Shader *shader;

    XformMatrix *ctm; //current transformation matrix
    int simpCodes(std::string word);
    Vertex extractVertex(std::istringstream &simpStream);
};
#endif