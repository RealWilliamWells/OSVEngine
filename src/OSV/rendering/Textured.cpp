//
// Created by william on 30/01/23.
//

#include "OSV/rendering/Textured.h"

osv::Textured::Textured(tbd::Texture &textureData) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    unsigned char* data = new unsigned char[textureData.data.size()];
    std::copy(textureData.data.begin(), textureData.data.end(), data);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData.width, textureData.height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    bindTexture();

    delete[] data;
}

void osv::Textured::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, texture);
}