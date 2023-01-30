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

    allocateBuffers();

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, RenderEngine::framebufferSizeCallback);
}

void RenderEngine::allocateBuffers() {
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

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void RenderEngine::addShaders(Shader newShader) {
    shaders.push_back(newShader);
}

void RenderEngine::addShaders(std::vector<Shader> newShaders) {
    for (Shader& shader : newShaders) {
        addShaders(shader);
    }
}

void RenderEngine::useShaders() {
    for (Shader& shader : shaders) {
        shader.use();
        shader.setFloat("someUniform", 1.0f);
    }
}

void RenderEngine::renderShaders() {
    useShaders();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool RenderEngine::update() {
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

void RenderEngine::processInput() {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void RenderEngine::renderScreen() {
    // TODO: Implement screen class for rendering engine to display

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Test entry point and use of render
int main() {
    RenderEngine* renderEngine = new RenderEngine();
    std::vector<Shader> shaders;

    renderEngine->initWindow();
    renderEngine->openWindow();

    Shader shader("res/shaders/defaultVertex.vs", "res/shaders/defaultFragment.vs");
    renderEngine->addShaders(shader);

    while (renderEngine->update());

    return 0;
}