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
    class LookInput;

    static bool firstMouse = true;
    static const float mouseSensitivity = 0.1f;
    static const float joyStickSensitivity = .7f;
}

class osv::LookInput {
public:
    static float lastX;
    static float lastY;

    static float yaw;
    static float pitch;


    static void mouseInputCallback(GLFWwindow* window, double xpos, double ypos) {
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

        updateYawAndPitch(xoffset, yoffset);
    }

    static void joyStickInputHandler(float xAxis, float yAxis) {
        float xoffset = joyStickSensitivity * xAxis;
        float yoffset = joyStickSensitivity * yAxis;

        updateYawAndPitch(xoffset, yoffset);
    }

    static void updateYawAndPitch(float& xoffset, float& yoffset) {
        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch =  89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    }
};

float osv::LookInput::lastX = 0.f;
float osv::LookInput::lastY = 0.f;
float osv::LookInput::yaw = -90.f;
float osv::LookInput::pitch = 0.f;

#endif //WIZENGINE3D_MOUSE_H
