//
// Created by william on 11/07/23.
//

#ifndef OSVENGINE_KEYINPUTHANDLER_H
#define OSVENGINE_KEYINPUTHANDLER_H

#include <GLFW/glfw3.h>
#include <functional>
#include <map>

class test{};

typedef void (* KeyActionCallback)();

namespace osv::KeyInputHandler {
    struct KeyAction {
        KeyActionCallback keyActionCallback;
    };

    struct InputMode {
        std::map<unsigned int, KeyAction> binds;

        GLFWcursorposfun mousePosCallback = MouseInput::EditMode::mouseInputCallback;
    };

    // TODO: Allow button combinations
    std::vector<InputMode> mainInputs;
    std::vector<InputMode> switchingInputs;
    unsigned int currentSwitchingBind = 0;

    float delta = 0.f;

    float timeSinceLastPressed = 0.f;
    float pressDelay = 0.5f;

    void addBindings(InputMode bind) {
        mainInputs.push_back(bind);
    }

    void addSwitchingBindings(InputMode bind) {
        switchingInputs.push_back(bind);
    }

    void checkAndActInput(GLFWwindow* window, std::map<unsigned int, KeyAction> keyBinds) {
        for (auto keyBind : keyBinds) {
            unsigned int key = keyBind.first;
            auto action = keyBind.second.keyActionCallback;

            if (glfwGetKey(window, key) == GLFW_PRESS) {
                action();
            }
        }
    }

    void processInput(GLFWwindow* window, float &deltaTime) {
        delta = deltaTime;
        timeSinceLastPressed += deltaTime;

        for (auto& inputMode : mainInputs) {
            checkAndActInput(window, inputMode.binds);

        }

        checkAndActInput(window, switchingInputs.at(currentSwitchingBind).binds);
    }

    bool delayPress() {
        if (timeSinceLastPressed >= pressDelay) {
            timeSinceLastPressed = 0.f;
            return false;
        }

        return true;
    }
}

#endif //OSVENGINE_KEYINPUTHANDLER_H
