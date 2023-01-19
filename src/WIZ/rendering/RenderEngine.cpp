#include "WIZ/rendering/RenderEngine.h"

#include <iostream>

void RenderEngine::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void RenderEngine::openWindow() {
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // TODO: throw window creation exception
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, RenderEngine::framebufferSizeCallback);
}

bool RenderEngine::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window)){
        glfwTerminate();
        return false;
    }
    return true;
}

int main() {
    RenderEngine* renderEngine = new RenderEngine();

    renderEngine->initWindow();
    renderEngine->openWindow();

    while (renderEngine->update());

    return 0;
}