//
// Created by william on 30/01/23.
//

#include "OSV/rendering/Camera.h"

osv::Camera::Camera() {
}

void osv::Camera::update(float delta, float pitch, float yaw) {
    look(pitch, yaw);

    deltaMoveSpeed = moveSpeed*delta;

    glm::vec3 yUnit = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(yUnit, front));

    up = glm::cross(front, right);
}

const glm::vec3 &osv::Camera::getPosition() const {
    return position;
}

const glm::vec3 &osv::Camera::getFront() const {
    return front;
}

const glm::vec3 &osv::Camera::getUp() const {
    return up;
}

void osv::Camera::moveFrontAndBack(bool dir) {
    float sign = dir ? 1.f : -1.f;

    position += sign * deltaMoveSpeed * front;
}

void osv::Camera::moveSideways(bool dir) {
    float sign = dir ? -1.f : 1.f;

    position += sign * glm::normalize(glm::cross(front, up)) * deltaMoveSpeed;
}

void osv::Camera::look(float pitch, float yaw) {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw));

    front = glm::normalize(direction);
}