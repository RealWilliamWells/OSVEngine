//
// Created by william on 29/01/23.
//

#ifndef WIZENGINE3D_VERTEXSHAPE_H
#define WIZENGINE3D_VERTEXSHAPE_H

#include "Shader.h"
#include "Textured.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace wiz {
    class VertexShape;
}

class wiz::VertexShape : public Shader, public Textured {
    float *vertices;
    unsigned int *indices;

public:
    VertexShape(float vertices[], unsigned int indices[], unsigned verticesSize, unsigned indicesSize,
                const char* vertexPath, const char* fragmentPath, const char *textureFile);

    void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};


#endif //WIZENGINE3D_VERTEXSHAPE_H
