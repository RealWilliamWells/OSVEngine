#include "OSV/rendering/RenderEngine.h"
#include <iostream>
#include "OSV/audio/Music.h"
#include "OSV/rendering/Model.h"
#include "OSV/input/MouseInput.h"

//#include "imgui.h"
//#include "backends/imgui_impl_glfw.h"
//#include "backends/imgui_impl_opengl3.h"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void osv::RenderEngine::openWindow() {
    window = glfwCreateWindow(1024, 768, "OSVEngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // TODO: throw window creation exception
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glViewport(0, 0, 1024, 768);

    glfwSetFramebufferSizeCallback(window, osv::RenderEngine::framebufferSizeCallback);

//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO &io = ImGui::GetIO();
//
//    // Setup Platform/Renderer bindings
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 300 es");
//
//    // Setup Dear ImGui style
//    ImGui::StyleColorsDark();
}

void osv::RenderEngine::updateCoordinateSystem() {
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void osv::RenderEngine::addModel(osv::Model newModel) {
    models.push_back(newModel);
}

void osv::RenderEngine::addLight(osv::Light& light) {
    lights.push_back(light);
}

void osv::RenderEngine::renderModels() {
    for (Model& shape : models) {
        shape.render(view, projection, renderOverrideMode, lights.at(0)); // For now only support one light source
    }
}

void osv::RenderEngine::clearBuffers() {
    for (Model& shape : models) {
        shape.deleteBuffers();
    }
}

bool osv::RenderEngine::update() {
    updateCoordinateSystem();
    updateView();

    renderScreen();

    glfwSwapBuffers(window);
    glfwPollEvents();

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.update(deltaTime);

    if (glfwWindowShouldClose(window)){
//        ImGui_ImplOpenGL3_Shutdown();
//        ImGui_ImplGlfw_Shutdown();
//        ImGui::DestroyContext();

        clearBuffers();
        glfwTerminate();
        return false;
    }
    return true;
}

void osv::RenderEngine::updateView() {
#ifndef __EMSCRIPTEN__
//    GLFWgamepadstate state;
//    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
//    float xAxis = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
//    float yAxis = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

//    if (std::abs(xAxis) > 0.2 || std::abs(yAxis) > 0.2) {
//        osv::MouseInput::FreeFly::joyStickInputHandler(xAxis, -yAxis);
//    }
#endif

    view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp()) * worldOrientation;
}

void osv::RenderEngine::renderScreen() {
    // TODO: Implement screen class for rendering engine to display
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderModels();

//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//
//    ImGui::Begin("Demo window");
//    ImGui::Button("Hello!");
//    ImGui::End();
//
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void osv::RenderEngine::addDisplayGrid(std::shared_ptr<Shader> shader, float width, float height) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    float stepSize = .1f;

    std::vector<Vertex> vertices;
    Vertex vertex;

    for(float j=0.f; j<=height; j++) {
        for(int i=0.f; i<=width; i++) {
            vertex.position = glm::vec3(i*stepSize - (width/2.f) * stepSize, 0.f, j*stepSize - (height/2.f) * stepSize);
            vertices.push_back(vertex);
        }
    }

    std::vector<unsigned int> indices;
    for(int j=0; j<height; ++j) {
        for(int i=0; i<width; ++i) {
            int row1 = j * (width+1);
            int row2 = (j+1) * (width+1);

            indices.push_back(row1+i);
            indices.push_back(row1+i+1);
            indices.push_back(row1+i+1);
            indices.push_back(row2+i+1);

            indices.push_back(row2+i+1);
            indices.push_back(row2+i);
            indices.push_back(row2+i);
            indices.push_back(row1+i);

        }
    }

    Model grid(shader, false, false);

    grid.addMesh(vertices, indices, {0.75f, 1.f, 0.f, 1.f}, GL_LINES);
 
    addModel(grid);
}

void osv::RenderEngine::setRenderOverrideMode(GLenum renderOverrideMode) {
    RenderEngine::renderOverrideMode = renderOverrideMode;
}

void osv::RenderEngine::relativeScaleModel(unsigned int index,glm::vec3 scale) {
    models.at(index).scaleRelative(scale);
}

void osv::RenderEngine::rotateModel(unsigned int index, float angle, glm::vec3 rotation) {
    models.at(index).rotate(angle, rotation);
}

void osv::RenderEngine::translateModel(unsigned int index, glm::vec3 translation) {
    models.at(index).translate(translation);
}

void osv::RenderEngine::setModelPos(unsigned int index, glm::vec3 position) {
    models.at(index).setPosition(position);
}

const std::vector<osv::Model> &osv::RenderEngine::getModels() const {
    return models;
}

void osv::RenderEngine::closeWindow() {
    glfwSetWindowShouldClose(window, true);
}

void osv::RenderEngine::setCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(window, callback);
}

osv::Camera *osv::RenderEngine::getCamera() {
    return &camera;
}

GLFWwindow *osv::RenderEngine::getWindow() const {
    return window;
}

void osv::RenderEngine::toggleMouseRelease() {
    captureMouse = !captureMouse;
    int value = captureMouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
    glfwSetInputMode(window, GLFW_CURSOR, value);
}

void osv::RenderEngine::orientateWorld(float angle, glm::vec3 rotation) {
    worldOrientation = glm::rotate(worldOrientation, angle, rotation);
}

void osv::RenderEngine::resetWorldOrientation() {
    worldOrientation = glm::mat4(1.f);
}
