//
// Created by william on 13/07/23.
//

#include "OSV/input/KeyInputHandler.h"

std::vector<osv::InputMode> osv::KeyInputHandler::switchingInputs;
unsigned int osv::KeyInputHandler::currentSwitchingBind = 0;

osv::KeyInputHandler::KeyInputHandler(std::shared_ptr<osv::RenderEngine> renderEngine) {
    this->renderEngine = renderEngine;
}

void osv::KeyInputHandler::checkAndActInput(GLFWwindow* window, InputMode binds) {
    checkAndActKeys(window, binds.keyBinds);

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
    checkAndActButtons(state, binds.joyButtonBinds);
    checkAndActAxes(state, binds.joyAxisBinds);
}

void osv::KeyInputHandler::checkAndActKeys(GLFWwindow* window, std::map<unsigned int, KeyAction>& binds) {


    for (auto keyBind : binds) {
        unsigned int key = keyBind.first;
        auto action = keyBind.second.keyActionCallback;

        if (glfwGetKey(window, key) == GLFW_PRESS) {
            action(renderEngine, delayPress(), delta);
        }
    }
}

void osv::KeyInputHandler::checkAndActButtons(GLFWgamepadstate& state, std::map<unsigned int, KeyAction>& binds) {
    for (auto keyBind : binds) {
        unsigned int key = keyBind.first;
        auto action = keyBind.second.keyActionCallback;

        if (state.buttons[key] == GLFW_PRESS) {
            action(renderEngine, delayPress(), delta);
        }
    }
}

void osv::KeyInputHandler::checkAndActAxes(GLFWgamepadstate& state, std::map<unsigned int, KeyAction>& binds) {
    for (auto keyBind : binds) {
        unsigned int key = keyBind.first;
        auto action = keyBind.second.keyActionCallback;
        bool dir = keyBind.second.joyStickDir;

        if (abs(state.axes[key] - (dir ? 0.5f : -0.5f)) > 0) {
            action(renderEngine, delayPress(), delta);
        }
    }
}

bool osv::KeyInputHandler::processInput(GLFWwindow* window, float& deltaTime) {
    delta = deltaTime;
    timeSinceLastPressed += deltaTime;

    for (auto& inputMode : mainInputs) {
        checkAndActInput(window, inputMode);
    }

    checkAndActInput(window, switchingInputs.at(currentSwitchingBind));

    return true;
}

void osv::KeyInputHandler::addBindings(InputMode bind) {
    mainInputs.push_back(bind);
}

void osv::KeyInputHandler::addSwitchingBindings(InputMode bind) {
    switchingInputs.push_back(bind);
}

bool osv::KeyInputHandler::delayPress() {
    if (timeSinceLastPressed >= pressDelay) {
        timeSinceLastPressed = 0.f;
        return false;
    }

    return true;
}

const std::vector<osv::InputMode>& osv::KeyInputHandler::getSwitchingInputs() const {
    return switchingInputs;
}

float osv::KeyInputHandler::getDelta() const {
    return delta;
}
