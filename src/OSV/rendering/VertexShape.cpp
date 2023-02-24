//
// Created by william on 29/01/23.
//

#include "OSV/rendering/VertexShape.h"

osv::VertexShape::VertexShape(float vertices[], unsigned int indices[], unsigned verticesSize, unsigned indicesSize,
                              const char* vertexPath, const char* fragmentPath, const char *textureFile) :
                              Shader(vertexPath, fragmentPath), Textured(textureFile) {
    setupBuffers();

    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    if (indicesSize)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void osv::VertexShape::setupBuffers() {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void osv::VertexShape::deleteBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void osv::VertexShape::render(glm::mat4 view, glm::mat4 projection) {
    // Rotate cube
    model = glm::mat4(1.0f);
    translate(glm::vec3(1.f, 1.0f, 0.0f));
    rotate((float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

    int modelLoc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(VAO);
}

void osv::VertexShape::translate(glm::vec3 translation) {
    model = glm::translate(model, translation);
}

void osv::VertexShape::rotate(float angle, glm::vec3 rotation) {
    model = glm::rotate(model, angle, rotation);
}
