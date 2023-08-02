//
// Created by william on 09/07/23.
//

#ifndef OSVENGINE_MESH_H
#define OSVENGINE_MESH_H

#include <vector>
#include "Shader.h"
#include "vec4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "fwd.hpp"
#include "Light.h"

namespace osv {
    struct Vertex;
    struct Texture;
    class Mesh;
}

struct osv::Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords = {0.f, 0.f};
};

struct osv::Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class osv::Mesh {
private:
    unsigned int VAO, VBO, EBO;

    GLenum mode;

    void setupMesh();

    bool modeCanBeOverridden;

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    glm::vec4 color;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, GLenum mode,
         bool &modeCanBeOverridden);

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec4 color, GLenum mode,
         bool &modeCanBeOverridden);

    void render(Shader &shader, glm::mat4 &view, glm::mat4 &projection, glm::mat4 &model, GLenum& overrideMode,
                bool useLighting, osv::Light& light);

    void deleteBuffers();
};


#endif //OSVENGINE_MESH_H
