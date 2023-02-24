#include "OSV/rendering/RenderEngine.h"
#include "OSV/input/LookInput.h"

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
#ifndef __EMSCRIPTEN__
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
#endif
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void osv::RenderEngine::openWindow() {
    window = glfwCreateWindow(1280, 720, "OSVEngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // TODO: throw window creation exception
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, osv::LookInput::mouseInputCallback);

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

    glViewport(0, 0, 1280, 720);

    glfwSetFramebufferSizeCallback(window, osv::RenderEngine::framebufferSizeCallback);
}

void osv::RenderEngine::updateCoordinateSystem() {
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
        shape.render(view, projection);
    }
}

void osv::RenderEngine::clearBuffers() {
    for (VertexShape& shape : vertexShapes) {
        shape.deleteBuffers();
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
        clearBuffers();
        glfwTerminate();
        return false;
    }
    return true;
}

void osv::RenderEngine::processInput() {
    float leftYAxis = 0.f;
    float leftXAxis = 0.f;

    char buttonB = 0;
    char buttonA = 0;

    char buttonStart = 0;

#ifndef __EMSCRIPTEN__
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    leftYAxis = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
    leftXAxis = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];

    buttonB = state.buttons[GLFW_GAMEPAD_BUTTON_B];
    buttonA = state.buttons[GLFW_GAMEPAD_BUTTON_A];

    buttonStart = state.buttons[GLFW_GAMEPAD_BUTTON_START];
#endif

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || leftYAxis < -0.5f)
        camera.moveFrontAndBack(true);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS  || leftYAxis > 0.5f)
        camera.moveFrontAndBack(false);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || leftXAxis < -0.5f)
        camera.moveSideways(true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || leftXAxis > 0.5f)
        camera.moveSideways(false);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || buttonB == GLFW_PRESS)
        camera.moveUpAndDown(true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || buttonA == GLFW_PRESS)
        camera.moveUpAndDown(false);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || buttonStart == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void osv::RenderEngine::updateView() {
#ifndef __EMSCRIPTEN__
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
    float xAxis = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
    float yAxis = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

    if (std::abs(xAxis) > 0.2 || std::abs(yAxis) > 0.2) {
        osv::LookInput::joyStickInputHandler(xAxis, -yAxis);
    }
#endif

    camera.update(deltaTime, osv::LookInput::pitch, osv::LookInput::yaw);
    view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
}

void osv::RenderEngine::renderScreen() {
    // TODO: Implement screen class for rendering engine to display
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderVerticesShapes();
}

// Test entry point and use of render
std::shared_ptr<osv::RenderEngine> renderEngine;

#ifdef __EMSCRIPTEN__
    static void emscriptenMainLoop() {
        renderEngine->update();
    }
#endif

int main() {
#ifdef OS_SWITCH
    userAppInit();
#endif
    renderEngine = std::shared_ptr<osv::RenderEngine>(new osv::RenderEngine());
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

    userAppExit();
#else
    while (renderEngine->update());
#endif

    return 0;
}