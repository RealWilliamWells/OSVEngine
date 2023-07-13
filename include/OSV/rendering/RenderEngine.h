//
// Created by william on 18/01/23.
//

#ifndef WIZENGINE3D_RENDERENGINE_H
#define WIZENGINE3D_RENDERENGINE_H

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

#include <memory>
#include <vector>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"


namespace osv {
    class RenderEngine;
}

class osv::RenderEngine {
private:
    GLFWwindow* window;

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    glm::mat4 view;
    glm::mat4 projection;

    Camera camera;

    std::vector<osv::Model> models;

    float lastFrame = 0.f;

    std::shared_ptr<Shader> mainShader;

    GLenum renderOverrideMode = GL_TRIANGLES;

    bool captureMouse = true;

public:
    float deltaTime = 0.f;

    RenderEngine();

    void initWindow();

    void openWindow();

    void updateCoordinateSystem();

    void addModel(osv::Model newModel);

    void addModels(std::vector<osv::Model> newModels);

    void renderModels();

    void clearBuffers();

    bool update();

    void processInput();

    void updateView();

    void renderScreen();

    void setMainShader(const std::shared_ptr<Shader> &mainShader);

    void addDisplayGrid();

    void setRenderOverrideMode(GLenum renderOverrideMode);

    void relativeScaleModel(unsigned int index, glm::vec3 scale);

    void rotateModel(unsigned int index, float angle, glm::vec3 rotation);

    void translateModel(unsigned int index, glm::vec3 translation);

    void setModelPos(unsigned int index, glm::vec3 position);

    const std::vector<osv::Model> &getModels() const;

    void closeWindow();

    void setCursorPosCallback(GLFWcursorposfun callback);

    Camera *getCamera();

    GLFWwindow *getWindow() const;

    void toggleMouseRelease();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

#endif //WIZENGINE3D_RENDERENGINE_H
