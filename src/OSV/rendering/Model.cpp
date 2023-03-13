//
// Created by william on 29/01/23.
//

#include "OSV/rendering/Model.h"

osv::Model::Model(std::vector<tbd::Vector3<float>> &vertices, std::vector<tbd::Vector3<tbd::U32>> &indices,
                  std::vector<tbd::Vector3<float>> &textureCoords, const char* vertexPath, const char* fragmentPath,
                  tbd::Texture &texture) : Shader(vertexPath, fragmentPath),
                  Textured(texture) {
    setupBuffers();

    // Load data into array buffer
    float *data = new float[5*vertices.size()];
    for (int i = 0; i<vertices.size(); i++) {
        data[0+i*5] = vertices.at(i).x;
        data[1+i*5] = vertices.at(i).y;
        data[2+i*5] = vertices.at(i).z;
        data[3+i*5] = textureCoords.at(i).x;
        data[4+i*5] = textureCoords.at(i).y;
    }

    glBufferData(GL_ARRAY_BUFFER, 5*vertices.size()*sizeof(float), data, GL_STATIC_DRAW);
    delete[] data;
//    if (indices.size() > 0)
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indicesData, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void osv::Model::setupBuffers() {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void osv::Model::deleteBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void osv::Model::render(glm::mat4 view, glm::mat4 projection) {
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

void osv::Model::translate(glm::vec3 translation) {
    model = glm::translate(model, translation);
}

void osv::Model::rotate(float angle, glm::vec3 rotation) {
    model = glm::rotate(model, angle, rotation);
}
