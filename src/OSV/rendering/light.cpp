//
// Created by william on 28/07/23.
//

#include "OSV/rendering/Light.h"

osv::Light::Light(glm::vec3 lightColor, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 position) :
                    lightColor(lightColor), diffuse(diffuse), ambient(ambient), position(position) {
}