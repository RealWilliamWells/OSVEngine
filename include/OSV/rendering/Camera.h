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
    glm::vec3 position = glm::vec3(0.0f, 1.0f, 6.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right;
    glm::vec3 up;

    float defaultMoveSpeed = 2.5f;
    float delta = 0.f;

public:
    Camera();

    void update(float delta);

    const glm::vec3 &getPosition() const;

    void setPosition(const glm::vec3 &position);

    const glm::vec3 &getFront() const;

    void setFront(const glm::vec3 &front);

    const glm::vec3 &getUp() const;

    void moveFrontAndBack(float speed);

    void moveSideways(float speed);

    void moveUpAndDown(float speed);

    void look(float pitch, float yaw);

    float getDefaultMoveSpeed() const;
};

#endif //WIZENGINE3D_CAMERA_H
