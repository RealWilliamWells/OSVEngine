//
// Created by william on 18/01/23.
//

#ifndef WIZENGINE3D_RENDERENGINE_H
#define WIZENGINE3D_RENDERENGINE_H

#include "Shader.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <vector>

namespace wiz {
    class RenderEngine;
}

class wiz::RenderEngine {
    GLFWwindow* window;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    std::vector<wiz::Shader> shaders;

public:
    void initWindow();

    void openWindow();

    void allocateBuffers();

    void addShaders(wiz::Shader newShader);

    void addShaders(std::vector<wiz::Shader> newShaders);

    void useShaders();

    void renderShaders();

    bool update();

    void processInput();

    void renderScreen();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

#endif //WIZENGINE3D_RENDERENGINE_H
