//
// Created by william on 30/01/23.
//

#ifndef WIZENGINE3D_CAMERA_H
#define WIZENGINE3D_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace wiz {
    class Camera;
}

class wiz::Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp = glm::cross(direction, cameraRight);

public:
    Camera();

    void update();
};

#endif //WIZENGINE3D_CAMERA_H
