//
// Created by william on 30/01/23.
//

#ifndef WIZENGINE3D_MOUSE_H
#define WIZENGINE3D_MOUSE_H

#include "GLFW/glfw3.h"
#include "OSV/rendering/Camera.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace osv {
    class MouseInput;

    static bool firstMouse = true;
    static const float mouseSensitivity = 0.1f;
    static const float joyStickSensitivity = .7f;

    namespace Mouse {
        static Camera *camera;
    }

}

class osv::MouseInput {
private:
    static void calcMouseOffsets(double &xpos, double &ypos, float& xoffset, float& yoffset) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        xoffset = xpos - lastX;
        yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;
    }

public:
    static float lastX;
    static float lastY;
    static float yaw;
    static float pitch;

    static void editModeInputCallback(GLFWwindow* window, double xpos, double ypos) {
        float xoffset;
        float yoffset;

        calcMouseOffsets(xpos, ypos, xoffset, yoffset);

        GLFWgamepadstate state;
#ifndef __EMSCRIPTEN__
        glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
#endif

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS) {
            yaw += xoffset;
        } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) || state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS) {
            pitch += yoffset;
        } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS)) {
            Mouse::camera->moveFrontAndBack(yoffset);
        }
    }

    static void freeFlyInputCallback(GLFWwindow* window, double xpos, double ypos) {
        float xoffset;
        float yoffset;

        calcMouseOffsets(xpos, ypos, xoffset, yoffset);

        yaw   += xoffset;
        pitch += yoffset;
        if(pitch > 89.0f)
            pitch =  89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    }
};

#endif //WIZENGINE3D_MOUSE_H
