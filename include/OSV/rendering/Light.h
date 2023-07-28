//
// Created by william on 28/07/23.
//

#ifndef OSVENGINE_LIGHT_H
#define OSVENGINE_LIGHT_H

#include "vec3.hpp"
#include "vec4.hpp"
#include "unit_models/Cube.h"

namespace osv {
    class Light;
}

class osv::Light : public model::Cube {
private:
    glm::vec3 lightColor;

public:
    Light(glm::vec4 objectColor, glm::vec3 lightColor);
};

#endif //OSVENGINE_LIGHT_H
