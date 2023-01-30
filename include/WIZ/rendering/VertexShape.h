//
// Created by william on 29/01/23.
//

#ifndef WIZENGINE3D_VERTEXSHAPE_H
#define WIZENGINE3D_VERTEXSHAPE_H

#include "GL/glew.h"

namespace wiz {
    class VertexShape;
}

class wiz::VertexShape {
    float *vertices;
    unsigned int *indices;

public:
    VertexShape(float vertices[], unsigned int indices[], unsigned verticesSize, unsigned indicesSize);
};


#endif //WIZENGINE3D_VERTEXSHAPE_H
