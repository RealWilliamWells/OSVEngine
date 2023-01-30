//
// Created by william on 29/01/23.
//

#include "WIZ/rendering/VertexShape.h"

wiz::VertexShape::VertexShape(float vertices[], unsigned int indices[], unsigned verticesSize, unsigned indicesSize,
                              const char* vertexPath, const char* fragmentPath, const char *textureFile) :
                              Shader(vertexPath, fragmentPath), Textured(textureFile),
                              vertices(vertices), indices(indices) {
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    if (indicesSize)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, this->indices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void wiz::VertexShape::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    int modelLoc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    bindTexture();
    useShader();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}