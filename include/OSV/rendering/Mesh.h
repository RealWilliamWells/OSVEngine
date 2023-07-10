//
// Created by william on 09/07/23.
//

#ifndef OSVENGINE_MESH_H
#define OSVENGINE_MESH_H

#include <vector>
#include "Shader.h"
#include "vec3.hpp"
#include "vec2.hpp"
#include "fwd.hpp"

namespace osv {
    struct Vertex;
    struct Texture;
    class Mesh;
}

struct osv::Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct osv::Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class osv::Mesh {
private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void render(Shader &shader, glm::mat4 &view, glm::mat4 &projection, glm::mat4 &model);

    void deleteBuffers();
};


#endif //OSVENGINE_MESH_H
