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

#include "Base.h"

namespace osv {
    class Model;
}

class osv::Model : public Shader, public Textured {
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::mat4 model = glm::mat4(1.0f);

public:
    Model(std::vector<tbd::Vector3<float>> &vertices, std::vector<tbd::Vector3<tbd::U32>> &indices,
          std::vector<tbd::Vector3<float>> &textureCoords, const char* vertexPath, const char* fragmentPath,
          tbd::Texture &texture);

    void setupBuffers();

    void deleteBuffers();

    void render(glm::mat4 view, glm::mat4 projection);

    void translate(glm::vec3 translation);

    void rotate(float angle, glm::vec3 rotation);
};


#endif //WIZENGINE3D_VERTEXSHAPE_H
