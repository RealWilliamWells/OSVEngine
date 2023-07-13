//
// Created by william on 13/07/23.
//

#include "OSV/input/KeyInputHandler.h"

std::vector<osv::InputMode> osv::KeyInputHandler::switchingInputs;
unsigned int osv::KeyInputHandler::currentSwitchingBind = 0;

osv::KeyInputHandler::KeyInputHandler(std::shared_ptr<osv::RenderEngine> renderEngine) {
    this->renderEngine = renderEngine;
}

void osv::KeyInputHandler::checkAndActInput(GLFWwindow* window, std::map<unsigned int, KeyAction> keyBinds) {
    for (auto keyBind : keyBinds) {
        unsigned int key = keyBind.first;
        auto action = keyBind.second.keyActionCallback;

        if (glfwGetKey(window, key) == GLFW_PRESS) {
            action(renderEngine, delayPress(), delta);
        }
    }
}

bool osv::KeyInputHandler::processInput(GLFWwindow* window, float& deltaTime) {
    delta = deltaTime;
    timeSinceLastPressed += deltaTime;

    for (auto& inputMode : mainInputs) {
        checkAndActInput(window, inputMode.binds);
    }

    checkAndActInput(window, switchingInputs.at(currentSwitchingBind).binds);

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
