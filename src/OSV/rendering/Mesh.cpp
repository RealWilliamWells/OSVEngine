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

void osv::Mesh::render(Shader &shader, glm::mat4 &view, glm::mat4 &projection, glm::mat4 &model, GLenum& overrideMode,
                       bool useLighting, Light& light) {
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

//        shader.setInt(("material." + name + number).c_str(), i); // TODO: Lighting and materials are not in shader code yet
        glBindTexture(GL_TEXTURE_2D, textures.at(i).id);
    }
    glActiveTexture(GL_TEXTURE0);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    shader.setBool("useLighting", useLighting);

    if (useLighting) {
        shader.setVec3("light.spotDir", {0.f, 0.f, -1.f});
        shader.setFloat("light.spotCutOff", .5f);

        shader.setVec3("light.ambient", light.ambient != glm::vec3{0.f, 0.f, 0.f} ? light.ambient : glm::vec3{0.1f, 0.1f, 0.1f}); // TODO: handle ambience better when no presence of light exists
        shader.setVec3("light.diffuse", light.diffuse);
        shader.setVec3("light.color", light.lightColor);
        shader.setVec3("light.position", light.position);

        // Use these default values for now
        shader.setVec3("material.diffuse", {1.0f, 0.5f, 0.31f});
        shader.setVec3("material.specular", {0.1f, 0.1f, 0.1f});
        shader.setFloat("material.shininess", 32.0f);
    } else {
        shader.setVec4("objectColor", color);
    }

    // draw mesh
    glBindVertexArray(VAO);

    // Allow rendering mode to be overridden except for models that should not be affected
    GLenum setMode = modeCanBeOverridden != false ? overrideMode : mode;
    if (indices.size() != 0) {
        glDrawElements(setMode, indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(setMode, 0, vertices.size());
    }
    glBindVertexArray(0);
}

void osv::Mesh::deleteBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}