#include "OSV/rendering/RenderEngine.h"
#include "OSV/input/LookInput.h"
#include "OSV/audio/Music.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>

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
    vertexShapes.push_back(newModel);
}

void osv::RenderEngine::addModels(std::vector<osv::Model> newModels) {
    for (Model& model : newModels) {
        addModel(model);
    }
}

void osv::RenderEngine::renderVerticesShapes() {
    for (Model& shape : vertexShapes) {
        shape.render(*mainShader, view, projection);
    }
}

void osv::RenderEngine::clearBuffers() {
    for (Model& shape : vertexShapes) {
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
//        ImGui_ImplOpenGL3_Shutdown();
//        ImGui_ImplGlfw_Shutdown();
//        ImGui::DestroyContext();

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

void osv::RenderEngine::setMainShader(const std::shared_ptr<Shader> &mainShader) {
    RenderEngine::mainShader = mainShader;
}

void osv::RenderEngine::addDisplayGrid() {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    float gridSize = 100.f; // Size of the grid
    float stepSize = .1f;

    std::vector<Vertex> vertices;
    Vertex vertex;

    for(float j=0.f; j<=gridSize; j++) {
        for(int i=0.f; i<=gridSize; i++) {
            vertex.position = glm::vec3(i*stepSize - (gridSize/2.f) * stepSize, 0.f, j*stepSize - (gridSize/2.f) * stepSize);
            vertices.push_back(vertex);
        }
    }

    std::vector<unsigned int> indices;
    for(int j=0; j<gridSize; ++j) {
        for(int i=0; i<gridSize; ++i) {
            int row1 = j * (gridSize+1);
            int row2 = (j+1) * (gridSize+1);

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


//    // Draw lines along the X axis
//    for (float i = -gridSize; i <= gridSize; i += stepSize) {
//        vertex.position = {i, 0.0f, -gridSize};
//        vertices.push_back(vertex);
//        vertex.position = {i, 0.0f, gridSize};
//        vertices.push_back(vertex);
//    }
//
//    // Draw lines along the Z axis
//    for (float i = -gridSize; i <= gridSize; i += stepSize) {
//        vertex.position = {-gridSize, 0.0f, i};
//        vertices.push_back(vertex);
//        vertex.position = {gridSize, 0.0f, i};
//        vertices.push_back(vertex);
//    }
//
//    std::vector<unsigned int> indices;
//    for(int j=0; j<gridSize; ++j) {
//        for(int i=0; i<gridSize; ++i) {
//
//            int row1 =  j    * (gridSize+1);
//            int row2 = (j+1) * (gridSize+1);
//
//            indices.push_back(row1+i);
//            indices.push_back(row1+i+1);
//            indices.push_back(row1+i+1);
//            indices.push_back(row2+i+1);
//
//            indices.push_back(row2+i+1);
//            indices.push_back(row2+i);
//            indices.push_back(row2+i);
//            indices.push_back(row1+i);
//        }
//    }

    Model grid;

    std::vector<Texture> textures;

    grid.addMesh(vertices, indices, textures, GL_LINES);

    addModel(grid);
}

//void osv::RenderEngine::setScene(tbd::Scene &scene, const char *vertexShaderFile, const char *fragmentShaderFile) {
//    currentScene = &scene;
//
//    const std::vector<tbd::Entity>& entities = currentScene->getEntities();
//
//    for (const tbd::Entity &entity : entities) {
//        Model model(entity.model->vertices, entity.model->indices, entity.model->textureCoords,
//                             vertexShaderFile, fragmentShaderFile, entity.model->texture);
//        addVerticesShapes(model);
//    }
//}