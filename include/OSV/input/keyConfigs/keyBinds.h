//
// Created by william on 11/07/23.
//

#ifndef OSVENGINE_FREEFLYBINDS_H
#define OSVENGINE_FREEFLYBINDS_H

#include <functional>
#include <map>
#include <GLFW/glfw3.h>
#include "OSV/rendering/Camera.h"

#include "OSV/input/KeyInputHandler.h"

namespace osv::KeyBinds {
    namespace WindowControl {
        GLFWwindow *window = nullptr;

        void closeWindow() {
            glfwSetWindowShouldClose(window, true);
        }
    }

    std::map<unsigned int, KeyActionCallback> generateWindowBinds(GLFWwindow *win) {
        WindowControl::window = win;

        std::map<unsigned int, KeyActionCallback> windowBinds;

        windowBinds[GLFW_KEY_ESCAPE] = WindowControl::closeWindow;

        return windowBinds;
    }

    namespace CameraControl {
        Camera *cam = nullptr;

        void moveCamForward() {
            cam->moveFrontAndBack(true);
        }

        void moveCamBack() {
            cam->moveFrontAndBack(false);
        }

        void moveCamLeft() {
            cam->moveSideways(true);
        }

        void moveCamRight() {
            cam->moveSideways(false);
        }

        void moveCamUp() {
            cam->moveUpAndDown(true);
        }

        void moveCamDown() {
            cam->moveUpAndDown(false);
        }
    }

    std::map<unsigned int, KeyActionCallback> generateFreeFlyBinds(Camera &camera) {
        std::map<unsigned int, KeyActionCallback> freeFly;

        CameraControl::cam = &camera;

        freeFly[GLFW_KEY_W] = CameraControl::moveCamForward;
        freeFly[GLFW_KEY_S] = CameraControl::moveCamBack;

        freeFly[GLFW_KEY_A] = CameraControl::moveCamLeft;
        freeFly[GLFW_KEY_D] = CameraControl::moveCamRight;

        freeFly[GLFW_KEY_LEFT_SHIFT] = CameraControl::moveCamUp;
        freeFly[GLFW_KEY_SPACE] = CameraControl::moveCamDown;

        return freeFly;
    }
}

#endif //OSVENGINE_FREEFLYBINDS_H
