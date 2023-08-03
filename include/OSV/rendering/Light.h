//
// Created by william on 28/07/23.
//

#ifndef OSVENGINE_LIGHT_H
#define OSVENGINE_LIGHT_H

#include "vec3.hpp"
#include "vec4.hpp"

namespace osv {
    class Light;
}

class osv::Light {
public:
    glm::vec3 spotDir = {0.f, 0.f, -1.f};
    float spotCutOff = .5f;

    glm::vec3 lightColor;
    glm::vec3 diffuse;
    glm::vec3 ambient;

    glm::vec3 position;

    Light(glm::vec3 lightColor, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 position);
};

#endif //OSVENGINE_LIGHT_H
