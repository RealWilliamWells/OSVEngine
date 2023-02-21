//
// Created by william on 30/01/23.
//

#ifndef WIZENGINE3D_MOUSE_H
#define WIZENGINE3D_MOUSE_H

#include "GLFW/glfw3.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace osv {
    class Mouse;

    static bool firstMouse = true;
    static const float mouseSensitivity = 0.1f;
}

class osv::Mouse {
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

float osv::Mouse::lastX = 0.f;
float osv::Mouse::lastY = 0.f;
float osv::Mouse::yaw = -90.f;
float osv::Mouse::pitch = 0.f;

#endif //WIZENGINE3D_MOUSE_H
