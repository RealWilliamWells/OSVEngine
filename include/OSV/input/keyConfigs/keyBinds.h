//
// Created by william on 11/07/23.
//

#ifndef OSVENGINE_FREEFLYBINDS_H
#define OSVENGINE_FREEFLYBINDS_H

#include <functional>
#include <map>
#include "OSV/input/keyConfigs/configStructs.h"
#include "OSV/input/KeyInputHandler.h"
#include "OSV/rendering/RenderEngine.h"

namespace osv::KeyBinds {
    float delta = 0.f;

    float timeSinceLastPressed = 0.f;
    float pressDelay = 0.5f;

    namespace WindowControl {
        void closeWindow(std::shared_ptr<RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->closeWindow();
        }

        void swapControls(std::shared_ptr<RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress)
                return;

            KeyInputHandler::currentSwitchingBind++;

            KeyInputHandler::currentSwitchingBind = KeyInputHandler::currentSwitchingBind >=
                    KeyInputHandler::switchingInputs.size() ? 0 : KeyInputHandler::currentSwitchingBind;
            renderEngine->setCursorPosCallback(KeyInputHandler::switchingInputs.at(KeyInputHandler::currentSwitchingBind).mousePosCallback);
        }

        void setPointsRendering(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->setRenderOverrideMode(GL_POINTS);
        }

        void setLinesRendering(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->setRenderOverrideMode(GL_LINES);
        }

        void setTrianglesRendering(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->setRenderOverrideMode(GL_TRIANGLES);
        }

        void releaseMouse(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->toggleMouseRelease();
        }
    }

    InputMode generateWindowBinds() {
        InputMode windowInputs;

        windowInputs.binds[GLFW_KEY_ESCAPE].keyActionCallback = WindowControl::closeWindow;
        windowInputs.binds[GLFW_KEY_Q].keyActionCallback = WindowControl::swapControls;

        windowInputs.binds[GLFW_KEY_P].keyActionCallback = WindowControl::setPointsRendering;
        windowInputs.binds[GLFW_KEY_L].keyActionCallback = WindowControl::setLinesRendering;
        windowInputs.binds[GLFW_KEY_T].keyActionCallback = WindowControl::setTrianglesRendering;

        windowInputs.binds[GLFW_KEY_LEFT_CONTROL].keyActionCallback = WindowControl::releaseMouse;

        return windowInputs;
    }

    namespace EditModeControl {
        unsigned int selectedModel = 1;
        float currentScale = 1.f;

        void swapModels(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress) {
                return;
            }

            const std::vector<Model> &models = renderEngine->getModels();

            selectedModel++;
            selectedModel = selectedModel >= models.size() ? 0 : selectedModel;

            currentScale = 1.f;
        }

        void scaleUp(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            currentScale += .25f * delta;

            renderEngine->relativeScaleModel(selectedModel, {currentScale, currentScale, currentScale});
        }

        void scaleDown(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            currentScale -= .25f * delta;

            renderEngine->relativeScaleModel(selectedModel, {currentScale, currentScale, currentScale});
        }

        void rotateLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress) {
                return;
            }

            float rotation = 8.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }

        void rotateRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress) {
                return;
            }

            float rotation = -8.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }

        void moveRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->translateModel(selectedModel,{.5f*delta, 0.f, 0.f});
        }

        void moveLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->translateModel(selectedModel,{-.5f*delta, 0.f, 0.f});
        }

        void moveUp(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->translateModel(selectedModel,{0.f, 0.f, .5f*delta});
        }

        void moveDown(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->translateModel(selectedModel,{0.f, 0.f, -.5f*delta});
        }

        void randPos(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress)
                return;

            renderEngine->setModelPos(selectedModel, {rand() % 20 - 10, 0.f, rand() % 20 - 10});
        }
    }

    InputMode generateEditModeBinds() {
        InputMode modelBinds;

        modelBinds.binds[GLFW_KEY_E].keyActionCallback = EditModeControl::swapModels;

        modelBinds.binds[GLFW_KEY_U].keyActionCallback = EditModeControl::scaleUp;
        modelBinds.binds[GLFW_KEY_J].keyActionCallback = EditModeControl::scaleDown;

        modelBinds.binds[GLFW_KEY_Z].keyActionCallback = EditModeControl::rotateLeft;
        modelBinds.binds[GLFW_KEY_X].keyActionCallback = EditModeControl::rotateRight;

        modelBinds.binds[GLFW_KEY_D].keyActionCallback = EditModeControl::moveRight;
        modelBinds.binds[GLFW_KEY_A].keyActionCallback = EditModeControl::moveLeft;
        modelBinds.binds[GLFW_KEY_W].keyActionCallback = EditModeControl::moveUp;
        modelBinds.binds[GLFW_KEY_S].keyActionCallback = EditModeControl::moveDown;

        modelBinds.binds[GLFW_KEY_SPACE].keyActionCallback = EditModeControl::randPos;

        modelBinds.mousePosCallback = MouseInput::editModeInputCallback;

        return modelBinds;
    }

    namespace CameraControl {
        void moveCamForward(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveFrontAndBack(camera->getDefaultMoveSpeed());
        }

        void moveCamBack(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveFrontAndBack(-camera->getDefaultMoveSpeed());
        }

        void moveCamLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveSideways(-camera->getDefaultMoveSpeed());
        }

        void moveCamRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveSideways(camera->getDefaultMoveSpeed());
        }

        void moveCamUp(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveUpAndDown(camera->getDefaultMoveSpeed());
        }

        void moveCamDown(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            Camera* camera = renderEngine->getCamera();
            camera->moveUpAndDown(-camera->getDefaultMoveSpeed());
        }
    }

    InputMode generateFreeFlyBinds() {
        InputMode freeFly;

        freeFly.binds[GLFW_KEY_W].keyActionCallback = CameraControl::moveCamForward;
        freeFly.binds[GLFW_KEY_S].keyActionCallback = CameraControl::moveCamBack;

        freeFly.binds[GLFW_KEY_A].keyActionCallback = CameraControl::moveCamLeft;
        freeFly.binds[GLFW_KEY_D].keyActionCallback = CameraControl::moveCamRight;

        freeFly.binds[GLFW_KEY_LEFT_SHIFT].keyActionCallback = CameraControl::moveCamUp;
        freeFly.binds[GLFW_KEY_SPACE].keyActionCallback = CameraControl::moveCamDown;

        freeFly.mousePosCallback = MouseInput::freeFlyInputCallback;

        return freeFly;
    }
}

#endif //OSVENGINE_FREEFLYBINDS_H
