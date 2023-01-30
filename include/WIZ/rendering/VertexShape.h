//
// Created by william on 29/01/23.
//

#ifndef WIZENGINE3D_VERTEXSHAPE_H
#define WIZENGINE3D_VERTEXSHAPE_H

#include "Textured.h"

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace wiz {
    class VertexShape;
}

class wiz::VertexShape : public Textured {
    float *vertices;
    unsigned int *indices;

public:
    VertexShape(float vertices[], unsigned int indices[], unsigned verticesSize, unsigned indicesSize,
                const char *textureFile);

    void render();
};


#endif //WIZENGINE3D_VERTEXSHAPE_H
