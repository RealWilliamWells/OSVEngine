//
// Created by william on 18/01/23.
//

#ifndef WIZENGINE3D_RENDERENGINE_H
#define WIZENGINE3D_RENDERENGINE_H

#include "Shader.h"
#include "VertexShape.h"
#include "Textured.h"

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

    // TODO: all shapes and other renderables should be in a scene or screen class that the rendering engine will draw
    std::vector<wiz::VertexShape> vertexShapes;

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection;


public:
    void initWindow();

    void openWindow();

    void setupBuffers();

    void updateCoordinateSystem();

    void addVerticesShapes(wiz::VertexShape newShape);

    void addVerticesShapes(std::vector<wiz::VertexShape> newShapes);

    void renderVerticesShapes();

    bool update();

    void processInput();

    void renderScreen();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

#endif //WIZENGINE3D_RENDERENGINE_H
