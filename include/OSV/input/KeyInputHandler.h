//
// Created by william on 11/07/23.
//

#ifndef OSVENGINE_KEYINPUTHANDLER_H
#define OSVENGINE_KEYINPUTHANDLER_H

#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include "OSV/input/keyConfigs/configStructs.h"
#include "OSV/input/MouseInput.h"

namespace osv {
    class RenderEngine;
    class KeyInputHandler;
}

class osv::KeyInputHandler {
private:
    std::shared_ptr<osv::RenderEngine> renderEngine;
    std::vector<InputMode> mainInputs;
    float delta = 0.f;
    float timeSinceLastPressed = 0.f;
    float pressDelay = 0.5f;

    void checkAndActInput(GLFWwindow* window, std::map<unsigned int, KeyAction> keyBinds);

public:
    static std::vector<InputMode> switchingInputs;
    static unsigned int currentSwitchingBind;

    KeyInputHandler(std::shared_ptr<osv::RenderEngine> renderEngine);

    bool processInput(GLFWwindow* window, float& deltaTime);

    void addBindings(InputMode bind);

    void addSwitchingBindings(InputMode bind);

    bool delayPress();

    const std::vector<InputMode>& getSwitchingInputs() const;

    float getDelta() const;
};

#endif //OSVENGINE_KEYINPUTHANDLER_H
