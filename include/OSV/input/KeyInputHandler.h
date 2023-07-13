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
    // TODO: Allow button combinations
    std::vector<std::map<unsigned int, KeyActionCallback>> keyBinds;
    std::vector<std::map<unsigned int, KeyActionCallback>> switchingKeyBinds;
    unsigned int currentSwitchingBind = 0;

    float delta = 0.f;

    float timeSinceLastPressed = 100.f;
    float pressDelay = .500f;

    void addBindings(std::map<unsigned int, KeyActionCallback> bind) {
        keyBinds.push_back(bind);
    }

    void addSwitchingBindings(std::map<unsigned int, KeyActionCallback> bind) {
        switchingKeyBinds.push_back(bind);
    }

    void checkAndActInput(GLFWwindow* window, std::map<unsigned int, KeyActionCallback> keyBinds) {
        for (auto keyBind : keyBinds) {
            unsigned int key = keyBind.first;
            auto action = keyBind.second;

            if (glfwGetKey(window, key) == GLFW_PRESS) {
                action();
            }
        }
    }

    void processInput(GLFWwindow* window, float &deltaTime) {
        delta = deltaTime;
        timeSinceLastPressed += deltaTime;

        for (auto& bind : keyBinds) {
            checkAndActInput(window, bind);

        }

        checkAndActInput(window, switchingKeyBinds.at(currentSwitchingBind));
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
