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

#include "Scene.h"

#include <vector>

namespace osv {
    class Textured;
}

class osv::Textured {
    unsigned int texture;

public:
    Textured(tbd::Texture &textureData);

    void bindTexture();
};

#endif //WIZENGINE3D_TEXTURED_H
