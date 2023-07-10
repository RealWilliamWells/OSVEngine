//
// Created by william on 29/01/23.
//

#ifndef WIZENGINE3D_VERTEXSHAPE_H
#define WIZENGINE3D_VERTEXSHAPE_H

#ifdef OS_SWITCH
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif

#include "GLFW/glfw3.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

namespace osv {
    class Model;
}

class osv::Model {
private:
    std::vector<Texture> textures_loaded;

    std::vector<osv::Mesh> meshes;
    std::string directory;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::mat4 model = glm::mat4(1.0f); // TODO: Allow to be settable

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

public:
    Model(std::string path);

    void deleteBuffers();

    void render(Shader &shader, glm::mat4 &view, glm::mat4 &projection);

    void translate(glm::vec3 translation);

    void rotate(float angle, glm::vec3 rotation);
};


#endif //WIZENGINE3D_VERTEXSHAPE_H
