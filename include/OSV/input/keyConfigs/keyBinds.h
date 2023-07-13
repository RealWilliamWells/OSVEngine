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
            if (KeyInputHandler::delayPress())
                return;

            unsigned int &currentBind = KeyInputHandler::currentSwitchingBind;
            currentBind++;
            currentBind = currentBind >= KeyInputHandler::switchingInputs.size() ? 0 : currentBind;

            // Change mouse controls to suit
            glfwSetCursorPosCallback(window, KeyInputHandler::switchingInputs.at(currentBind).mousePosCallback);
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

    KeyInputHandler::InputMode generateWindowBinds(GLFWwindow *win, RenderEngine *renderEngine) {
        WindowControl::window = win;
        WindowControl::engine = renderEngine;

        KeyInputHandler::InputMode windowInputs;

        windowInputs.binds[GLFW_KEY_ESCAPE].keyActionCallback = WindowControl::closeWindow;
        windowInputs.binds[GLFW_KEY_Q].keyActionCallback = WindowControl::swapControls;

        windowInputs.binds[GLFW_KEY_P].keyActionCallback = WindowControl::setPointsRendering;
        windowInputs.binds[GLFW_KEY_L].keyActionCallback = WindowControl::setLinesRendering;
        windowInputs.binds[GLFW_KEY_T].keyActionCallback = WindowControl::setTrianglesRendering;

        return windowInputs;
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

    KeyInputHandler::InputMode generateEditModeBinds(RenderEngine *renderEngine) {
        EditModeControl::engine = renderEngine;

        KeyInputHandler::InputMode modelBinds;

        modelBinds.binds[GLFW_KEY_E].keyActionCallback = EditModeControl::swapModels;

        modelBinds.binds[GLFW_KEY_U].keyActionCallback = EditModeControl::scaleUp;
        modelBinds.binds[GLFW_KEY_J].keyActionCallback = EditModeControl::scaleDown;

        modelBinds.binds[GLFW_KEY_Z].keyActionCallback = EditModeControl::rotateLeft;
        modelBinds.binds[GLFW_KEY_X].keyActionCallback = EditModeControl::rotateRight;

        modelBinds.mousePosCallback = MouseInput::EditMode::mouseInputCallback;

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

    KeyInputHandler::InputMode generateFreeFlyBinds(Camera &camera) {
        KeyInputHandler::InputMode freeFly;

        CameraControl::cam = &camera;

        freeFly.binds[GLFW_KEY_W].keyActionCallback = CameraControl::moveCamForward;
        freeFly.binds[GLFW_KEY_S].keyActionCallback = CameraControl::moveCamBack;

        freeFly.binds[GLFW_KEY_A].keyActionCallback = CameraControl::moveCamLeft;
        freeFly.binds[GLFW_KEY_D].keyActionCallback = CameraControl::moveCamRight;

        freeFly.binds[GLFW_KEY_LEFT_SHIFT].keyActionCallback = CameraControl::moveCamUp;
        freeFly.binds[GLFW_KEY_SPACE].keyActionCallback = CameraControl::moveCamDown;

        freeFly.mousePosCallback = MouseInput::FreeFly::mouseInputCallback;

        return freeFly;
    }
}

#endif //OSVENGINE_FREEFLYBINDS_H
