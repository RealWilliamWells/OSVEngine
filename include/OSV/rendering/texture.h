//
// Created by william on 30/01/23.
//

#ifndef WIZENGINE3D_TEXTURED_H
#define WIZENGINE3D_TEXTURED_H

#ifdef OS_SWITCH
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <iostream>

namespace osv::texture {
    unsigned char *loadTextureFromFile(std::string &texturePath, int &width, int &height) {
        // TODO: use asset loader for this instead
        stbi_set_flip_vertically_on_load(true);

        int nrChannels;
        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

        return data;
    }

    unsigned int loadTexture(std::string relativePath, const std::string modelDirectory) {
        std::string texturePath = modelDirectory + "/" + relativePath;

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width;
        int height;
        unsigned char *data = loadTextureFromFile(texturePath, width, height);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, texture);

        return texture;
    }
}

#endif //WIZENGINE3D_TEXTURED_H
