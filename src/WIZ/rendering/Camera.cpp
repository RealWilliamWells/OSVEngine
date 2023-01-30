//
// Created by william on 30/01/23.
//

#include "WIZ/rendering/Camera.h"

wiz::Camera::Camera() {
}

void wiz::Camera::update(float delta, float pitch, float yaw) {
    look(pitch, yaw);

    deltaMoveSpeed = moveSpeed*delta;

    glm::vec3 yUnit = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(yUnit, front));

    up = glm::cross(front, right);
}

const glm::vec3 &wiz::Camera::getPosition() const {
    return position;
}

const glm::vec3 &wiz::Camera::getFront() const {
    return front;
}

const glm::vec3 &wiz::Camera::getUp() const {
    return up;
}

void wiz::Camera::moveFrontAndBack(bool dir) {
    float sign = dir ? 1.f : -1.f;

    position += sign * deltaMoveSpeed * front;
}

void wiz::Camera::moveSideways(bool dir) {
    float sign = dir ? -1.f : 1.f;

    position += sign * glm::normalize(glm::cross(front, up)) * deltaMoveSpeed;
}

void wiz::Camera::look(float pitch, float yaw) {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw));

    front = glm::normalize(direction);
}