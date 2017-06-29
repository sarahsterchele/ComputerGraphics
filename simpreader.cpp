#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <math.h>
#include "simpreader.h"

bool isFilled = true;

SimpReader::SimpReader(Drawable *drawable)
{
    this->drawable = drawable;
    this->zbuffer = new Z_Buffer();
    this->shader = new Shader();
    this->renderer = new Renderer(drawable, zbuffer, shader);
    ctm = new XformMatrix();
}

int SimpReader::simpCodes(std::string word)
{
    if (word == "{") return 0;
    if (word == "}") return 1;
    if (word == "scale") return 2;
    if (word == "rotate") return 3;
    if (word == "translate") return 4;
    if (word == "line") return 5;
    if (word == "polygon") return 6;
    if (word == "mesh") return 7;
    if (word == "wire") return 8;
    if (word == "filled") return 9;
    if (word == "camera") return 10;
    if (word == "light") return 11;
    if (word == "surface") return 12;
    if (word == "ambient") return 13;
    if (word == "phong") return 14;
    if (word == "gouraud") return 15;
    if (word == "flat") return 16;

    return -1;
}

void SimpReader::readSimpFile(const char* filename)
{
    std::string line, word, file;

    std::ifstream simpFile(filename);
  
    if (simpFile.is_open())
    {
        while (getline(simpFile, line))
        {
            std::istringstream simpStream(line);
            simpStream >> std::skipws >> word;

            switch(simpCodes(word))
            {
                case 0: // {
                    ctm->pushCTM();
                    break;
                case 1: // }
                    ctm->popCTM();      
                    break;
                case 2: //scale
                {
                    float x, y, z;

                    simpStream >> std::skipws >> x;
                    simpStream >> std::skipws >> y;
                    simpStream >> std::skipws >> z;

                    ctm->scale(x, y, z);
                    break;
                }
                case 3: //rotate
                {
                    char axis;
                    std::string degree;

                    simpStream >> std::skipws >> axis;
                    simpStream >> std::skipws >> degree;

                    switch(axis)
                    {
                    case 'X':
                        ctm->rotateX(std::atof(degree.c_str()));
                        break;
                    case 'Y':
                        ctm->rotateY(std::atof(degree.c_str()));
                        break;
                    case 'Z':
                        ctm->rotateZ(std::atof(degree.c_str()));
                        break;
                    }
                    break;
                }
                case 4: //translate
                {
                    float x, y, z;

                    simpStream >> std::skipws >> x;
                    simpStream >> std::skipws >> y;
                    simpStream >> std::skipws >> z;
                    ctm->translate(x, y, z);
                    break;
                }
                case 5:  //line
                {   
                    Vertex lineVertices[2];

                    for (int i = 0; i < 2; i++)
                    {                       
                        lineVertices[i] = extractVertex(simpStream);
                    }

                    renderer->draw_Line
                    (
                        lineVertices[0], 
                        lineVertices[1]
                    );
                
                    break;
                }
                case 6: //polygon
                {           
    
                    Vertex polygonVertices[3];

                    for (int i = 0; i < 3; i++)
                    {
                        polygonVertices[i] = extractVertex(simpStream);
                    }
                    renderer->draw_Polygon
                    (
                        polygonVertices[0], 
                        polygonVertices[1], 
                        polygonVertices[2],
                        isFilled
                    );
                    break;
                }
                case 7:  //mesh
                {
                    bool localSurface = false;
                    
                    std::string file;
                    std::string word;
                    float r, g, b, ks, alpha;
                    char character;
                    int cols, rows;

                    //get file name and open
                    simpStream >> std::skipws >> file; 
                    std::ifstream meshFile(file.c_str());

                    //Get the first line
                    getline(meshFile, line);
                    std::istringstream meshStream(line);

                    meshStream >> std::skipws >> word;
                    if (word == "surface")
                    {
                        meshStream >> std::skipws >> character; // remove Paren

                        meshStream >> std::skipws >> word;  // get r
                        word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                        r = std::atof(word.c_str());
                        meshStream >> std::skipws >> word; //get g
                        word.erase(std::remove(word.begin(), word.end(), ','), word.end());
                        g = std::atof(word.c_str());

                        meshStream >> std::skipws >> word; //get b
                        word.erase(std::remove(word.begin(), word.end(), ')'), word.end());
                        b = std::atof(word.c_str());
    
                        meshStream >> std::skipws >> ks;
                        meshStream >> std::skipws >> alpha;
                        
                        shader->addSurface(r, g, b, ks, alpha);
                        meshStream.clear();
                        getline(meshFile, line);
                        meshStream.str(line);
                        meshStream >> std::skipws >> word;

                    }
                    if (word == "normal")
                    {
                        shader->normalAveraging = true;
                        meshStream.clear();
                        getline(meshFile, line);
                        meshStream.str(line);
                        meshStream >> std::skipws >> word;
                    }

                    cols = atoi(word.c_str());

                    meshStream.clear();

                    // Get rows from the second line
                    getline(meshFile, line);
                    meshStream.str(line);
                    meshStream >> std::skipws >> rows;
                    meshStream.clear();

                    //For all other lines, put into a mesh grid.
                    std::vector <std::vector<Vertex> > mesh(rows, std::vector<Vertex>(cols));
                    for (int i = 0; i < rows; i++)
                    {
                        for (int j = 0; j < cols; j++)
                        {
                            getline(meshFile, line);
                            meshStream.str(line);
                            
                            mesh[i][j] = extractVertex(meshStream);
                             
                            meshStream.clear();
                        }
                    }
                    renderer->draw_Mesh(mesh, rows, cols, isFilled);

                    // Remove the mesh surface after use.
                    if (localSurface)
                        shader->removeSurface();
                    
                    meshFile.close();
                    break;
                }
                case 8: //wireframe
                    isFilled = false;
                    break;
                case 9:  //filled in polygons
                    isFilled = true;
                    break;
                case 10:  //place camera
                {
                    float xlow, ylow, xhigh, yhigh, hither, yon;
                    simpStream >> std::skipws >> xlow;
                    simpStream >> std::skipws >> ylow;
                    simpStream >> std::skipws >> xhigh;
                    simpStream >> std::skipws >> yhigh;
                    simpStream >> std::skipws >> hither;
                    simpStream >> std::skipws >> yon;
                    ctm->addCamera(xlow, ylow, xhigh, yhigh, hither, yon);
                    break;
                }
                case 11: // add a light
                {
                    float r, g, b, A, B;
                    simpStream >> std::skipws >> r;
                    simpStream >> std::skipws >> g;
                    simpStream >> std::skipws >> b;
                    simpStream >> std::skipws >> A;
                    simpStream >> std::skipws >> B;

                    shader->addLightSource(r, g, b, A, B, ctm->getWorldPosition(0, 0, 0));
                    break;
                }
                case 12: //add surface 
                {
                    float r, g, b, ks, alpha;
                    char character;
                    std::string value;

                    simpStream >> std::skipws >> character; // remove Paren

                    simpStream >> std::skipws >> value; // get first number
                    value.erase(std::remove(value.begin(), value.end(), ','), value.end());
                    r = std::atof(value.c_str());

                    simpStream >> std::skipws >> value;
                    value.erase(std::remove(value.begin(), value.end(), ','), value.end());
                    g = std::atof(value.c_str());

                    simpStream >> std::skipws >> value;
                    value.erase(std::remove(value.begin(), value.end(), ')'), value.end());
                    b = std::atof(value.c_str());
    
                    simpStream >> std::skipws >> ks;
                    simpStream >> std::skipws >> alpha;
                    shader->addSurface(r, g, b, ks, alpha);
                    break;
                }
                case 13:
                {
                    float r, g, b;
                    char character;
                    std::string value;

                    simpStream >> std::skipws >> character; // remove Paren

                    simpStream >> std::skipws >> value; // get first number
                    value.erase(std::remove(value.begin(), value.end(), ','), value.end());
                    r = std::atof(value.c_str());

                    simpStream >> std::skipws >> value;
                    value.erase(std::remove(value.begin(), value.end(), ','), value.end());
                    g = std::atof(value.c_str());

                    simpStream >> std::skipws >> value;
                    value.erase(std::remove(value.begin(), value.end(), ')'), value.end());
                    b = std::atof(value.c_str());
    
                    shader->setAmbient(r, g, b);
                    break;
                }
                case 14:
                    shader->setLightingStyle(PHONG);
                    break;

                case 15:
                    shader->setLightingStyle(GOURAUD);
                    break;
                case 16:
                    shader->setLightingStyle(FLAT);
                    break;
            }
            simpStream.clear();
            word = "";
        }
    }
    simpFile.close();
}

Vertex SimpReader::extractVertex(std::istringstream &simpStream)
{
    float x, y, z, nx = 0, ny = 0, nz = 0;
    float r = -1, g = -1, b = -1;
    bool hasNormal = false;
    char character;
    std::string value;

    simpStream >> std::skipws >> character; // remove Paren

    simpStream >> std::skipws >> value; // get first number
    value.erase(std::remove(value.begin(), value.end(), ','), value.end());
    x = std::atof(value.c_str());

    simpStream >> std::skipws >> value;
    value.erase(std::remove(value.begin(), value.end(), ','), value.end());
    y = std::atof(value.c_str());

    //Check if this is the last one or if there are rbg colors too.
    simpStream >> std::skipws >> value;
    if (*value.rbegin() == ')')
    {
        value.erase(std::remove(value.begin(), value.end(), ')'), value.end());
        z = std::atof(value.c_str());
    }
    else
    {
        value.erase(std::remove(value.begin(), value.end(), ','), value.end());
        z = std::atof(value.c_str());

        simpStream >> std::skipws >> value;
        value.erase(std::remove(value.begin(), value.end(), ','), value.end());
        r = std::atof(value.c_str());

        simpStream >> std::skipws >> value;
        value.erase(std::remove(value.begin(), value.end(), ','), value.end());
        g = std::atof(value.c_str());

        simpStream >> std::skipws >> value;
        value.erase(std::remove(value.begin(), value.end(), ')'), value.end());
        b = std::atof(value.c_str());
    }

    simpStream >> std::skipws >> character; // check for N

    if (character == 'n')
    {
        hasNormal = true;
        
        simpStream >> std::skipws >> character; // remove Open bracket

        simpStream >> std::skipws >> value; // get first number
        value.erase(std::remove(value.begin(), value.end(), ','), value.end());
        nx = std::atof(value.c_str());

        simpStream >> std::skipws >> value;
        value.erase(std::remove(value.begin(), value.end(), ','), value.end());
        ny = std::atof(value.c_str());

        simpStream >> std::skipws >> value;
        value.erase(std::remove(value.begin(), value.end(), ']'), value.end());
        nz = std::atof(value.c_str());
    }

    return ctm->getTransformedVertex(x, y, z, r, g, b, hasNormal, nx, ny, nz);
}