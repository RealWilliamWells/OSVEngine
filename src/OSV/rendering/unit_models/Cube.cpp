//
// Created by william on 28/07/23.
//

#include "OSV/rendering/unit_models/Cube.h"

osv::model::Cube::Cube(glm::vec4 color) : Model(true) {
    initCube(color);
}

void osv::model::Cube::initCube(glm::vec4 color) {
    std::vector<Vertex> vertices = osv::model::cube::vertices;
    std::vector<unsigned int> indices = {};

    for (Vertex& vertex : vertices) {
        vertex.texCoords = {0.f, 0.f}; // TODO: properly handle lack of textures
    }

    addMesh(vertices, indices, color, GL_TRIANGLES);
}