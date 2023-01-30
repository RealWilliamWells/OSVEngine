//
// Created by william on 30/01/23.
//

#ifndef WIZENGINE3D_MOUSE_H
#define WIZENGINE3D_MOUSE_H

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace wiz {
    class Mouse;

    static bool firstMouse = true;
    static const float mouseSensitivity = 0.1f;
}

class wiz::Mouse {
public:
    static float lastX;
    static float lastY;

    static float yaw;
    static float pitch;


    static void inputCallback(GLFWwindow* window, double xpos, double ypos) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch =  89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    }
};

float wiz::Mouse::lastX = 0.f;
float wiz::Mouse::lastY = 0.f;
float wiz::Mouse::yaw = -90.f;
float wiz::Mouse::pitch = 0.f;

#endif //WIZENGINE3D_MOUSE_H
