//
// Created by william on 29/01/23.
//

#include "OSV/rendering/Model.h"
#include "OSV/rendering/texture.h"

osv::Model::Model(std::shared_ptr<Shader> shader, bool renderCanBeOverridden, bool useLighting) :
    renderCanBeOverridden(renderCanBeOverridden), shader(shader), useLighting(useLighting) {
}

osv::Model::Model(std::shared_ptr<Shader> shader, std::string path, bool renderCanBeOverridden, glm::vec3 position, float angle, glm::vec3 rotation,
                  glm::vec3 scale, bool useLighting) : renderCanBeOverridden(renderCanBeOverridden), shader(shader),
                  useLighting(useLighting) {
    startingModel = glm::rotate(startingModel, angle, rotation);
    startingModel = glm::scale(startingModel, scale);

    model = startingModel;
    model = glm::translate(model, position);
    baseModel = model;

    this->position = position;

    loadModel(path);
}

void osv::Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

    import.FreeScene();

}

void osv::Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, &node->mTransformation));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

osv::Mesh osv::Model::processMesh(aiMesh *mesh, const aiScene *scene, const aiMatrix4x4 *transformation) {
    glm::mat3 rotationMat = {1.f, 1.f, 1.f,
                                   1.f, 1.f, 1.f,
                                   1.f, 1.f, 1.f};

    glm::vec3 translationVec = {1.f, 1.f, 1.f};

    if (transformation) {
        rotationMat = {transformation->a1, transformation->b1, transformation->c1,
                             transformation->a2, transformation->b2, transformation->c2,
                             transformation->a3, transformation->b3, transformation->c3,};

        translationVec = {transformation->a4, transformation->b4, transformation->c4};
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.position = (rotationMat * vertex.position) + translationVec;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                           aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
                                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures, GL_TRIANGLES, renderCanBeOverridden);
}

std::vector<osv::Texture> osv::Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded.at(j).path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip) {   //  If texture has not been loaded previously
            Texture texture;
            texture.id = texture::loadTexture(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

void osv::Model::render(glm::mat4 &view, glm::mat4 &projection, GLenum& overrideMode, Light& light) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes.at(i).render(*shader, view, projection, model, overrideMode, useLighting, light);
    }

    for (Model& child : childrenModels) {
        child.render(view, projection, overrideMode, light);
    }
}

void osv::Model::deleteBuffers() {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes.at(i).deleteBuffers();
    }
}

void osv::Model::translate(glm::vec3 translation) {
    model = glm::translate(model, translation);
    baseModel = model;

    for (Model& child : childrenModels) {
        child.translate(translation);
    }
}

void osv::Model::rotate(float angle, glm::vec3 rotation) {
    model = glm::rotate(model, angle, rotation);
    baseModel = model;

    for (Model& child : childrenModels) {
        child.translate(position-child.position);
        child.rotate(angle, rotation);
        child.translate(child.position-position);
    }
}

void osv::Model::scale(glm::vec3 scale) {
    model = glm::scale(baseModel, scale);
    baseModel = model;

    for (Model& child : childrenModels) {
        child.scale(scale);
    }
}

void osv::Model::scaleRelative(glm::vec3 scale) {
    model = glm::scale(baseModel, scale);

    for (Model& child : childrenModels) {
        child.scaleRelative(scale);
    }
}

void osv::Model::addMesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> &textures,
                         GLenum mode) {
    meshes.push_back(Mesh(vertices, indices, textures, mode, renderCanBeOverridden));
}

void osv::Model::addMesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, glm::vec4 color,
                         GLenum mode) {
    meshes.push_back(Mesh(vertices, indices, color, mode, renderCanBeOverridden));
}

void osv::Model::setPosition(glm::vec3 position) {
    this->position = position;
    model = glm::translate(startingModel, position);
    baseModel = model;
}

const glm::vec3 &osv::Model::getPosition() const {
    return position;
}

void osv::Model::addChild(osv::Model &model) {
    childrenModels.push_back(model);
}