//
// Created by william on 30/01/23.
//

#ifndef WIZENGINE3D_MOUSE_H
#define WIZENGINE3D_MOUSE_H

#include "GLFW/glfw3.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace osv::MouseInput {
    bool firstMouse = true;
    const float mouseSensitivity = 0.1f;
    const float joyStickSensitivity = .7f;

    float lastX = 0.f;
    float lastY = 0.f;
    float yaw = -90.f;
    float pitch = 0.f;

    namespace EditMode {
        void updateYawAndPitch(float& xoffset, float& yoffset) {
            yaw   += xoffset;
            pitch += yoffset;

            if(pitch > 89.0f)
                pitch =  89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;
        }

        void mouseInputCallback(GLFWwindow* window, double xpos, double ypos) {
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
    }

    namespace FreeFly {
        void updateYawAndPitch(float& xoffset, float& yoffset) {
            yaw   += xoffset;
            pitch += yoffset;

            if(pitch > 89.0f)
                pitch =  89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;
        }

        void mouseInputCallback(GLFWwindow* window, double xpos, double ypos) {
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

        void joyStickInputHandler(float xAxis, float yAxis) {
            float xoffset = joyStickSensitivity * xAxis;
            float yoffset = joyStickSensitivity * yAxis;

            updateYawAndPitch(xoffset, yoffset);
        }
    }
};



#endif //WIZENGINE3D_MOUSE_H
