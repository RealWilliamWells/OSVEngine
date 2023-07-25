//
// Created by william on 09/07/23.
//

#include "OSV/rendering/Mesh.h"
#include "gtc/type_ptr.hpp"

osv::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, GLenum mode,
                bool &modeCanBeOverridden) : modeCanBeOverridden(modeCanBeOverridden) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->mode = mode;

    setupMesh();
}

osv::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec4 color, GLenum mode,
                bool &modeCanBeOverridden) : modeCanBeOverridden(modeCanBeOverridden) {
    this->vertices = vertices;
    this->indices = indices;
    this->color = color;

    this->mode = mode;

    setupMesh();
}

void osv::Mesh::setupMesh() {
    glEnable(GL_PROGRAM_POINT_SIZE);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void osv::Mesh::render(Shader &shader, glm::mat4 &view, glm::mat4 &projection, glm::mat4 &model, GLenum& overrideMode) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        std::string number;
        std::string name = textures.at(i).type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setInt(("material." + name + number).c_str(), i); // TODO: Lighting and materials are not in shader code yet
        glBindTexture(GL_TEXTURE_2D, textures.at(i).id);
    }
    glActiveTexture(GL_TEXTURE0);

    int modelLoc = glGetUniformLocation(shader.programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(shader.programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(shader.programID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    int colorLoc = glGetUniformLocation(shader.programID, "Color");
    glUniform4f(colorLoc, color.x, color.y, color.z, color.w);

    // draw mesh
    glBindVertexArray(VAO);

    // Allow rendering mode to be overridden except for models that should not be affected
    GLenum setMode = modeCanBeOverridden != false ? overrideMode : mode;
    glDrawElements(setMode, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void osv::Mesh::deleteBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}