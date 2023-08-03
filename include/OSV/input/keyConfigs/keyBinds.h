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
            if (delayPress)
                return;

            renderEngine->toggleMouseRelease();
        }
    }

    InputMode generateWindowBinds() {
        InputMode windowInputs;

#ifndef OS_SWITCH
        windowInputs.keyBinds[GLFW_KEY_ESCAPE].keyActionCallback = WindowControl::closeWindow;
        windowInputs.keyBinds[GLFW_KEY_Q].keyActionCallback = WindowControl::swapControls;

        windowInputs.keyBinds[GLFW_KEY_P].keyActionCallback = WindowControl::setPointsRendering;
        windowInputs.keyBinds[GLFW_KEY_L].keyActionCallback = WindowControl::setLinesRendering;
        windowInputs.keyBinds[GLFW_KEY_T].keyActionCallback = WindowControl::setTrianglesRendering;

#ifndef __EMSCRIPTEN__
        windowInputs.keyBinds[GLFW_KEY_LEFT_CONTROL].keyActionCallback = WindowControl::releaseMouse;
#endif
#endif

        // Joystick controls
        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_START].keyActionCallback = WindowControl::closeWindow;
        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_BACK].keyActionCallback = WindowControl::swapControls;

//        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_LEFT].keyActionCallback = WindowControl::setPointsRendering;
        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT].keyActionCallback = WindowControl::setLinesRendering;
        windowInputs.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_LEFT].keyActionCallback = WindowControl::setTrianglesRendering;

        return windowInputs;
    }

    namespace EditModeControl {
        unsigned int selectedModel = 0;
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
            float rotation = 3.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }

        void rotateRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            float rotation = -3.f * delta;

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

        // World orientation control keyBinds
        void orientateRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->orientateWorld(1.f * delta, {0.0f, 1.f, .0f});
        }

        void orientateLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->orientateWorld(-1.f * delta, {0.f, 1.f, .0f});
        }

        void orientateUp(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->orientateWorld(1.f * delta, {1.f, 0.f, .0f});
        }

        void orientateDown(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->orientateWorld(-1.f * delta, {1.f, 0.f, .0f});
        }

        void resetOrientation(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            renderEngine->resetWorldOrientation();
        }
    }

    InputMode generateEditModeBinds() {
        InputMode modelBinds;

#ifndef OS_SWITCH
        modelBinds.keyBinds[GLFW_KEY_E].keyActionCallback = EditModeControl::swapModels;

        modelBinds.keyBinds[GLFW_KEY_U].keyActionCallback = EditModeControl::scaleUp;
        modelBinds.keyBinds[GLFW_KEY_J].keyActionCallback = EditModeControl::scaleDown;

        modelBinds.keyBinds[GLFW_KEY_Z].keyActionCallback = EditModeControl::rotateLeft;
        modelBinds.keyBinds[GLFW_KEY_X].keyActionCallback = EditModeControl::rotateRight;

        modelBinds.keyBinds[GLFW_KEY_D].keyActionCallback = EditModeControl::moveRight;
        modelBinds.keyBinds[GLFW_KEY_A].keyActionCallback = EditModeControl::moveLeft;
        modelBinds.keyBinds[GLFW_KEY_W].keyActionCallback = EditModeControl::moveUp;
        modelBinds.keyBinds[GLFW_KEY_S].keyActionCallback = EditModeControl::moveDown;

        modelBinds.keyBinds[GLFW_KEY_SPACE].keyActionCallback = EditModeControl::randPos;

        // World orientation control keybinds
        modelBinds.keyBinds[GLFW_KEY_RIGHT].keyActionCallback = EditModeControl::orientateRight;
        modelBinds.keyBinds[GLFW_KEY_LEFT].keyActionCallback = EditModeControl::orientateLeft;
        modelBinds.keyBinds[GLFW_KEY_UP].keyActionCallback = EditModeControl::orientateUp;
        modelBinds.keyBinds[GLFW_KEY_DOWN].keyActionCallback = EditModeControl::orientateDown;

        modelBinds.keyBinds[GLFW_KEY_HOME].keyActionCallback = EditModeControl::resetOrientation;
#endif

        // Joystick controls
        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_UP].keyActionCallback = EditModeControl::swapModels;

//        modelBinds.keyBinds[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER].keyActionCallback = EditModeControl::scaleUp;
//        modelBinds.keyBinds[GLFW_KEY_J].keyActionCallback = EditModeControl::scaleDown;

//        modelBinds.keyBinds[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER].keyActionCallback = EditModeControl::rotateLeft;
//        modelBinds.keyBinds[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER].keyActionCallback = EditModeControl::rotateRight;

//        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_A].keyActionCallback = EditModeControl::moveRight;
        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_Y].keyActionCallback = EditModeControl::moveLeft;
        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_X].keyActionCallback = EditModeControl::moveUp;
        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_B].keyActionCallback = EditModeControl::moveDown;

        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB].keyActionCallback = EditModeControl::randPos;

        // World orientation control keybinds
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_X].joyStickDir = true;
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_X].keyActionCallback = EditModeControl::orientateRight;

        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_X].joyStickDir = false;
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_X].keyActionCallback = EditModeControl::orientateLeft;

        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_Y].joyStickDir = true;
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_Y].keyActionCallback = EditModeControl::orientateUp;

        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_Y].joyStickDir = false;
        modelBinds.joyAxisBinds[GLFW_GAMEPAD_AXIS_LEFT_Y].keyActionCallback = EditModeControl::orientateDown;

        modelBinds.joyButtonBinds[GLFW_GAMEPAD_BUTTON_LEFT_THUMB].keyActionCallback = EditModeControl::resetOrientation;

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

#ifndef OS_SWITCH
        freeFly.keyBinds[GLFW_KEY_W].keyActionCallback = CameraControl::moveCamForward;
        freeFly.keyBinds[GLFW_KEY_S].keyActionCallback = CameraControl::moveCamBack;

        freeFly.keyBinds[GLFW_KEY_A].keyActionCallback = CameraControl::moveCamLeft;
        freeFly.keyBinds[GLFW_KEY_D].keyActionCallback = CameraControl::moveCamRight;

        freeFly.keyBinds[GLFW_KEY_LEFT_SHIFT].keyActionCallback = CameraControl::moveCamUp;
        freeFly.keyBinds[GLFW_KEY_SPACE].keyActionCallback = CameraControl::moveCamDown;
#endif

        // Joystick control
        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_X].keyActionCallback = CameraControl::moveCamForward;
        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_B].keyActionCallback = CameraControl::moveCamBack;

        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_Y].keyActionCallback = CameraControl::moveCamLeft;
        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_A].keyActionCallback = CameraControl::moveCamRight;

        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_UP].keyActionCallback = CameraControl::moveCamUp;
        freeFly.joyButtonBinds[GLFW_GAMEPAD_BUTTON_DPAD_DOWN].keyActionCallback = CameraControl::moveCamDown;

        freeFly.mousePosCallback = MouseInput::freeFlyInputCallback;

        return freeFly;
    }

    namespace Quiz1Control {
        unsigned int selectedModel = 3;

        void setNewCameraView(std::shared_ptr<osv::RenderEngine> renderEngine, int racketNum) {
            Camera* cam = renderEngine->getCamera();

            float behindOffset = 2.f;

            float xRacketPos = racketNum == 1 || racketNum == 2 ? 3.f : -3.f;

            float zRacketPos = racketNum == 1 || racketNum == 3 ? .8f : -.8f;

            float xBehindOffset = racketNum == 1 || racketNum == 2 ? behindOffset : -behindOffset;

            cam->setPosition({xRacketPos + xBehindOffset, 2.f, zRacketPos});

            glm::vec3 camLook = {-xRacketPos, -1.f, 0.f};

            cam->setFront(glm::normalize(camLook));

            osv::MouseInput::yaw = 0.f;
            osv::MouseInput::pitch = 0.f;
        }

        void selectRacket(std::shared_ptr<osv::RenderEngine> renderEngine, int racketNum) {
            selectedModel = 2 + racketNum;

            setNewCameraView(renderEngine, racketNum);
        }

        void selectRacket1(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            selectRacket(renderEngine, 1);
        }

        void selectRacket2(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            selectRacket(renderEngine, 2);
        }

        void selectRacket3(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            selectRacket(renderEngine, 3);
        }

        void selectRacket4(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            selectRacket(renderEngine, 4);
        }

        void rotateLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            float rotation = 2.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }

        void rotateRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            float rotation = -2.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }
    }

    InputMode generateQuiz1Binds() {
        InputMode quiz1Binds;

        quiz1Binds.keyBinds[GLFW_KEY_1].keyActionCallback = Quiz1Control::selectRacket1;
        quiz1Binds.keyBinds[GLFW_KEY_2].keyActionCallback = Quiz1Control::selectRacket2;
        quiz1Binds.keyBinds[GLFW_KEY_3].keyActionCallback = Quiz1Control::selectRacket3;
        quiz1Binds.keyBinds[GLFW_KEY_4].keyActionCallback = Quiz1Control::selectRacket4;

        quiz1Binds.keyBinds[GLFW_KEY_A].keyActionCallback = Quiz1Control::rotateLeft;
        quiz1Binds.keyBinds[GLFW_KEY_D].keyActionCallback = Quiz1Control::rotateRight;

        quiz1Binds.mousePosCallback = MouseInput::quiz1Callback;

        return quiz1Binds;
    }

    namespace Quiz2Control {
        unsigned int selectedModel = 0;

        void setNewCameraView(std::shared_ptr<osv::RenderEngine> renderEngine, int racketNum) {
            Camera* cam = renderEngine->getCamera();

            float behindOffset = 2.f;

            float xRacketPos = racketNum == 1 ? 3.f : -3.f;

            float xBehindOffset = racketNum == 1 ? behindOffset : -behindOffset;

            cam->setPosition({xRacketPos + xBehindOffset, 2.f, 0.f});

            glm::vec3 camLook = {-xRacketPos, -1.f, 0.f};

            cam->setFront(glm::normalize(camLook));

            osv::MouseInput::yaw = 0.f;
            osv::MouseInput::pitch = 0.f;
        }

        void swapRackets(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress)
                return;

            selectedModel = selectedModel == 0 ? 1 : 0;

            setNewCameraView(renderEngine, selectedModel+1);
        }

        void resetCameraToOrigin(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            if (delayPress)
                return;

            selectedModel = 0;

            Camera* cam = renderEngine->getCamera();
            cam->setPosition({0.0f, 1.0f, 6.0f});
            cam->setFront({0.0f, 0.0f, -1.0f});

            osv::MouseInput::yaw = 0.f;
            osv::MouseInput::pitch = 0.f;
        }

        void rotateLeft(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            float rotation = 2.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }

        void rotateRight(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta) {
            float rotation = -2.f * delta;

            renderEngine->rotateModel(selectedModel, rotation, {0.f, 1.f, 0.f});
        }
    }

    InputMode generateQuiz2Binds() {
        InputMode quiz1Binds;

        quiz1Binds.keyBinds[GLFW_KEY_M].keyActionCallback = Quiz2Control::swapRackets;
        quiz1Binds.keyBinds[GLFW_KEY_R].keyActionCallback = Quiz2Control::resetCameraToOrigin;

        quiz1Binds.keyBinds[GLFW_KEY_A].keyActionCallback = Quiz2Control::rotateLeft;
        quiz1Binds.keyBinds[GLFW_KEY_D].keyActionCallback = Quiz2Control::rotateRight;

        quiz1Binds.mousePosCallback = MouseInput::quiz1Callback;

        return quiz1Binds;
    }
}

#endif //OSVENGINE_FREEFLYBINDS_H
