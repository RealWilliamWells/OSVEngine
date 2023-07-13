//
// Created by william on 13/07/23.
//

#ifndef OSVENGINE_CONFIGSTRUCTS_H
#define OSVENGINE_CONFIGSTRUCTS_H

#include <map>
#include <memory>

namespace osv{
    class RenderEngine;
}

typedef void (* KeyActionCallback)(std::shared_ptr<osv::RenderEngine> renderEngine, bool delayPress, float delta);

namespace osv {
    struct KeyAction;
    struct InputMode;
}

struct osv::KeyAction {
    KeyActionCallback keyActionCallback;
};

struct osv::InputMode {
    std::map<unsigned int, KeyAction> binds;

    GLFWcursorposfun mousePosCallback;
};

#endif //OSVENGINE_CONFIGSTRUCTS_H
