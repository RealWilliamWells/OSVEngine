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
#include <memory>

#include "Mesh.h"
#include "Shader.h"

namespace osv {
    class Model;
}

class osv::Model {
private:
    std::shared_ptr<Shader> shader;

    std::vector<Model> childrenModels;

    std::vector<Texture> textures_loaded;

    std::vector<osv::Mesh> meshes;
    std::string directory;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 baseModel = glm::mat4(1.0f);

    glm::mat4 startingModel = glm::mat4(1.0f);

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, const aiMatrix4x4 *transformation);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    bool renderCanBeOverridden = true;
    bool useLighting;

    glm::vec3 position;

public:
    Model(std::shared_ptr<Shader> shader, std::string path, bool renderCanBeOverridden, glm::vec3 position, float angle,
          glm::vec3 rotation, glm::vec3 scale, bool useLighting);

    Model(std::shared_ptr<Shader> shader, bool renderCanBeOverridden, bool useLighting);

    void deleteBuffers();

    void render(glm::mat4 &view, glm::mat4 &projection, GLenum& overrideMode, Light& light);

    void translate(glm::vec3 translation);

    void rotate(float angle, glm::vec3 rotation);

    void scale(glm::vec3 scale);

    void scaleRelative(glm::vec3 scale);

    void addMesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> &textures,
                 GLenum mode);

    void addMesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, glm::vec4 color,
                 GLenum mode);

    void setPosition(glm::vec3 position);

    void addChild(Model& model);

    const glm::vec3 &getPosition() const;
};


#endif //WIZENGINE3D_VERTEXSHAPE_H
