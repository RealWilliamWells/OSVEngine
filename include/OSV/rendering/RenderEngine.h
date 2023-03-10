//
// Created by william on 18/01/23.
//

#ifndef WIZENGINE3D_RENDERENGINE_H
#define WIZENGINE3D_RENDERENGINE_H

#include <memory>

#include "Shader.h"
#include "Model.h"
#include "Textured.h"
#include "Camera.h"

#include "Scene.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3

#include <GL/gl.h>
#include <GLES3/gl3.h>
#elifdef OS_SWITCH
#include <glad/glad.h>
#else
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#endif

#include <vector>



namespace osv {
    class RenderEngine;
}

class osv::RenderEngine {
    GLFWwindow* window;

    // TODO: all shapes and other renderables should be in a scene or screen class that the rendering engine will draw
    std::vector<osv::Model> vertexShapes;

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    glm::mat4 view;
    glm::mat4 projection;

    Camera camera;

    float deltaTime = 0.f;
    float lastFrame = 0.f;

    tbd::Scene* currentScene;

public:
    RenderEngine();

    void initWindow();

    void openWindow();

    void updateCoordinateSystem();

    void addVerticesShapes(osv::Model newShape);

    void addVerticesShapes(std::vector<osv::Model> newShapes);

    void renderVerticesShapes();

    void clearBuffers();

    bool update();

    void processInput();

    void updateView();

    void renderScreen();

    void setScene(tbd::Scene &scene, const char *vertexShaderFile, const char *fragmentShaderFile);

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

#endif //WIZENGINE3D_RENDERENGINE_H
