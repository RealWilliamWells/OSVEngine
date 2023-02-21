//
// Created by william on 30/01/23.
//

#ifndef WIZENGINE3D_CAMERA_H
#define WIZENGINE3D_CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace osv {
    class Camera;
}

class osv::Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 4.0f);
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    float moveSpeed = 2.5f;
    float deltaMoveSpeed = 0.f;

public:
    Camera();

    void update(float delta, float pitch, float yaw);

    const glm::vec3 &getPosition() const;

    const glm::vec3 &getFront() const;

    const glm::vec3 &getUp() const;

    void moveFrontAndBack(bool dir); // Forwards: true | Backwards: false

    void moveSideways(bool dir); // Left: true | Right: false

    void moveUpAndDown(bool dir); // down: true | up: false

    void look(float pitch, float yaw);
};

#endif //WIZENGINE3D_CAMERA_H
