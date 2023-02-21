#include "OSV/rendering/RenderEngine.h"
#include "OSV/input/Mouse.h"

#include <iostream>

#ifndef OS_SWITCH
#define ASSET(_str) "./res/" _str
#else
#define ASSET(_str) "romfs:/" _str

#include <switch.h>
#include <unistd.h>

static int nxlink_sock = -1;

void userAppInit()
{
	romfsInit();
	socketInitializeDefault();
	nxlink_sock = nxlinkStdio();
}

void userAppExit()
{
	if (nxlink_sock != -1)
		close(nxlink_sock);
	socketExit();
	romfsExit();
}
#endif

osv::RenderEngine::RenderEngine() : camera() {
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
}

void osv::RenderEngine::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void osv::RenderEngine::openWindow() {
    window = glfwCreateWindow(800, 600, "OSVEngine Test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // TODO: throw window creation exception
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, osv::Mouse::inputCallback);

//    glewExperimental = true; // Needed for core profile
#ifdef OS_SWITCH
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
#else
    if (glewInit() != GLEW_OK) {
#endif
        std::cerr << "Failed to create GLEW or glad" << std::endl;
        glfwTerminate();
        // TODO: throw glew or glad init exception
    }

    glEnable(GL_DEPTH_TEST);

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    setupBuffers();

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, osv::RenderEngine::framebufferSizeCallback);
}

void osv::RenderEngine::setupBuffers() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void osv::RenderEngine::updateCoordinateSystem() {
    model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void osv::RenderEngine::addVerticesShapes(osv::VertexShape newShape) {
    vertexShapes.push_back(newShape);
}

void osv::RenderEngine::addVerticesShapes(std::vector<osv::VertexShape> newShapes) {
    for (VertexShape& shape : newShapes) {
        addVerticesShapes(shape);
    }
}

void osv::RenderEngine::renderVerticesShapes() {
    for (VertexShape& shape : vertexShapes) {
        shape.render(model, view, projection);
    }
}

bool osv::RenderEngine::update() {
    processInput();

    updateCoordinateSystem();
    updateView();

    renderScreen();

    glfwSwapBuffers(window);
    glfwPollEvents();

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwWindowShouldClose(window)){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glfwTerminate();
        return false;
    }
    return true;
}

void osv::RenderEngine::processInput() {
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveFrontAndBack(true);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveFrontAndBack(false);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveSideways(true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveSideways(false);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || state.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void osv::RenderEngine::updateView() {
    camera.update(deltaTime, osv::Mouse::pitch, osv::Mouse::yaw);
    view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
}

void osv::RenderEngine::renderScreen() {
    // TODO: Implement screen class for rendering engine to display
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);

    renderVerticesShapes();
}

// Test entry point and use of render
osv::RenderEngine* renderEngine;

#ifdef __EMSCRIPTEN__
    static void emscriptenMainLoop() {
        renderEngine->update();
    }
#endif

int main() {
    renderEngine = new osv::RenderEngine();
    std::vector<osv::Shader> shaders;

    renderEngine->initWindow();
    renderEngine->openWindow();

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    osv::VertexShape vertexShape(vertices, nullptr, sizeof(vertices), 0,
                                 ASSET("shaders/defaultVertex.vs"), ASSET("shaders/defaultFragment.fs"),
                                 ASSET("gfx/jesus.jpg"));

    renderEngine->addVerticesShapes(vertexShape);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(emscriptenMainLoop, 0, false);
#elifdef OS_SWITCH
    consoleInit(NULL);

    while (appletMainLoop()) {
        renderEngine->update();
    }
#else
    while (renderEngine->update());
#endif

    return 0;
}