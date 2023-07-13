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
        RenderEngine *engine = nullptr;

        void closeWindow() {
            glfwSetWindowShouldClose(window, true);
        }

        void swapControls() {
            unsigned int &currentBind = KeyInputHandler::currentSwitchingBind;
            currentBind++;
            currentBind = currentBind >= KeyInputHandler::switchingKeyBinds.size() ? 0 : currentBind;
        }

        void setPointsRendering() {
            engine->setRenderOverrideMode(GL_POINTS);
        }

        void setLinesRendering() {
            engine->setRenderOverrideMode(GL_LINES);
        }

        void setTrianglesRendering() {
            engine->setRenderOverrideMode(GL_TRIANGLES);
        }
    }

    std::map<unsigned int, KeyActionCallback> generateWindowBinds(GLFWwindow *win, RenderEngine *renderEngine) {
        WindowControl::window = win;
        WindowControl::engine = renderEngine;

        std::map<unsigned int, KeyActionCallback> windowBinds;

        windowBinds[GLFW_KEY_ESCAPE] = WindowControl::closeWindow;
        windowBinds[GLFW_KEY_Q] = WindowControl::swapControls;

        windowBinds[GLFW_KEY_P] = WindowControl::setPointsRendering;
        windowBinds[GLFW_KEY_L] = WindowControl::setLinesRendering;
        windowBinds[GLFW_KEY_T] = WindowControl::setTrianglesRendering;

        return windowBinds;
    }

    namespace EditModeControl {
        RenderEngine *engine = nullptr;
        unsigned int selectedModel = 1;
        float currentScale = 1.f;

        void swapModels() {
            if (KeyInputHandler::delayPress()) {
                return;
            }

            std::vector<Model> &models = engine->models;

            selectedModel++;
            selectedModel = selectedModel >= models.size() ? 0 : selectedModel;

            currentScale = 1.f;
        }

        void scaleUp() {
            std::vector<Model> &models = engine->models;
            currentScale += .25f * KeyInputHandler::delta;

            models.at(selectedModel).scaleRelative({currentScale, currentScale, currentScale});
        }

        void scaleDown() {
            std::vector<Model> &models = engine->models;
            currentScale -= .25f * KeyInputHandler::delta;

            models.at(selectedModel).scaleRelative({currentScale, currentScale, currentScale});
        }

        void rotateLeft() {
            if (KeyInputHandler::delayPress()) {
                return;
            }

            std::vector<Model> &models = engine->models;
            float rotation = 8.f * KeyInputHandler::delta;

            models.at(selectedModel).rotate(rotation, {0.f, 1.f, 0.f});
        }

        void rotateRight() {
            if (KeyInputHandler::delayPress()) {
                return;
            }

            std::vector<Model> &models = engine->models;
            float rotation = -8.f * KeyInputHandler::delta;

            models.at(selectedModel).rotate(rotation, {0.f, 1.f, 0.f});
        }
    }

    std::map<unsigned int, KeyActionCallback> generateEditModeBinds(RenderEngine *renderEngine) {
        EditModeControl::engine = renderEngine;

        std::map<unsigned int, KeyActionCallback> modelBinds;

        modelBinds[GLFW_KEY_E] = EditModeControl::swapModels;

        modelBinds[GLFW_KEY_U] = EditModeControl::scaleUp;
        modelBinds[GLFW_KEY_J] = EditModeControl::scaleDown;

        modelBinds[GLFW_KEY_Z] = EditModeControl::rotateLeft;
        modelBinds[GLFW_KEY_X] = EditModeControl::rotateRight;

        return modelBinds;
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
