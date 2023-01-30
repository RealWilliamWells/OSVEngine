//
// Created by william on 30/01/23.
//

#include "WIZ/rendering/Camera.h"

wiz::Camera::Camera() {
}

void wiz::Camera::update() {
    direction = glm::normalize(position - target);

    glm::vec3 yUnit = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraRight = glm::normalize(glm::cross(yUnit, direction));
}