//
// Created by william on 29/01/23.
//

#ifndef WIZENGINE3D_VERTEXSHAPE_H
#define WIZENGINE3D_VERTEXSHAPE_H

#include "Shader.h"
#include "Textured.h"

#ifdef OS_SWITCH
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif

#include "GLFW/glfw3.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace osv {
    class VertexShape;
}

class osv::VertexShape : public Shader, public Textured {
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    glm::mat4 model = glm::mat4(1.0f);

public:
    VertexShape(float vertices[], unsigned int indices[], unsigned verticesSize, unsigned indicesSize,
                const char* vertexPath, const char* fragmentPath, const char *textureFile);

    void setupBuffers();

    void deleteBuffers();

    void render(glm::mat4 view, glm::mat4 projection);
};


#endif //WIZENGINE3D_VERTEXSHAPE_H
