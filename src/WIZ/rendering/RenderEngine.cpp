#include "WIZ/rendering/RenderEngine.h"

#include <iostream>

void wiz::RenderEngine::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void wiz::RenderEngine::openWindow() {
    window = glfwCreateWindow(800, 600, "WizEngine3D Test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // TODO: throw window creation exception
    }
    glfwMakeContextCurrent(window);

//    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        // TODO: throw glew init exception
    }

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    setupBuffers();

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, wiz::RenderEngine::framebufferSizeCallback);
}

void wiz::RenderEngine::setupBuffers() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void wiz::RenderEngine::addShaders(Shader newShader) {
    shaders.push_back(newShader);
}

void wiz::RenderEngine::addShaders(std::vector<Shader> newShaders) {
    for (Shader& shader : newShaders) {
        addShaders(shader);
    }
}

void wiz::RenderEngine::useShaders() {
    for (Shader& shader : shaders) {
        shader.use();
        shader.setFloat("someUniform", 1.0f);
    }
}

void wiz::RenderEngine::addVerticesShapes(wiz::VertexShape newShape) {
    vertexShapes.push_back(newShape);
}

void wiz::RenderEngine::addVerticesShapes(std::vector<wiz::VertexShape> newShapes) {
    for (VertexShape& shader : newShapes) {
        addVerticesShapes(shader);
    }
}

void wiz::RenderEngine::renderShaders() {
    useShaders();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool wiz::RenderEngine::update() {
    processInput();

    renderScreen();
    renderShaders();

    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window)){
        glfwTerminate();
        return false;
    }
    return true;
}

void wiz::RenderEngine::processInput() {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void wiz::RenderEngine::renderScreen() {
    // TODO: Implement screen class for rendering engine to display

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Test entry point and use of render
int main() {
    wiz::RenderEngine* renderEngine = new wiz::RenderEngine();
    std::vector<wiz::Shader> shaders;

    renderEngine->initWindow();
    renderEngine->openWindow();

    wiz::Shader shader("res/shaders/defaultVertex.vs", "res/shaders/defaultFragment.vs");
    renderEngine->addShaders(shader);

    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };

    wiz::VertexShape vertexShape(vertices, indices, sizeof(vertices), sizeof(indices));

    renderEngine->addVerticesShapes(vertexShape);

    while (renderEngine->update());

    return 0;
}