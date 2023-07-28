//
// Created by william on 13/03/23.
//

#include "OSV/rendering/RenderEngine.h"
#include "OSV/input/KeyInputHandler.h"
#include "OSV/input/keyConfigs/keyBinds.h"

#ifndef OS_SWITCH
#define ASSET(_str) "./res/" _str
#else
#define ASSET(_str) "romfs:/" _str

#include <switch.h>
#include <unistd.h>

static int nxlink_sock = -1;

void userAppInit()
{
	romfsInit();
	socketInitializeDefault();
	nxlink_sock = nxlinkStdio();
}

void userAppExit()
{
	if (nxlink_sock != -1)
		close(nxlink_sock);
	socketExit();
	romfsExit();
}
#endif

std::shared_ptr<osv::RenderEngine> renderEngine;
std::shared_ptr<osv::Shader> shader;
std::shared_ptr<osv::KeyInputHandler> keyInputHandler;

#ifdef __EMSCRIPTEN__
static void emscriptenMainLoop() {
    renderEngine->update();
    keyInputHandler->processInput(renderEngine->getWindow(), renderEngine->deltaTime);
}
#endif

int main() {
#ifdef OS_SWITCH
    userAppInit();
#endif
    renderEngine = std::shared_ptr<osv::RenderEngine>(new osv::RenderEngine());
    std::vector<osv::Shader> shaders;

    renderEngine->initWindow();
    renderEngine->openWindow();

    shader = std::shared_ptr<osv::Shader>(new osv::Shader(ASSET("shaders/defaultVertex.fs"), ASSET("shaders/defaultFragment.fs")));

    renderEngine->setMainShader(shader);

    // Add models
    osv::Model coorModel(ASSET("models/coor_axis/coor_axis.dae"), false,
                         {0.f, 0.f, 0.f}, 0.f, {1.f, 1.f, 1.f}, {0.19f, 0.19f, 0.19f});

    osv::Model tennisBall(ASSET("models/tennis_ball/tennis_ball.dae"), true,
                         {3.f, 0.f, 0.f}, 0.f, {1.f, 1.f, 1.f}, {0.19f, 0.19f, 0.19f});

    renderEngine->addModel(coorModel);
    renderEngine->addModel(tennisBall);

    renderEngine->addDisplayGrid();

    // Input handling
    // TODO: remove this cringe
    osv::Mouse::camera = renderEngine->getCamera();

    keyInputHandler = std::shared_ptr<osv::KeyInputHandler>(new osv::KeyInputHandler(renderEngine));
    keyInputHandler->addBindings(osv::KeyBinds::generateWindowBinds());

    keyInputHandler->addSwitchingBindings(osv::KeyBinds::generateEditModeBinds());
    keyInputHandler->addSwitchingBindings(osv::KeyBinds::generateFreeFlyBinds());

    renderEngine->setCursorPosCallback(keyInputHandler->getSwitchingInputs().at(keyInputHandler->currentSwitchingBind).mousePosCallback);

//    Music backgroundMusic(ASSET("sfx/background.wav"));
//    backgroundMusic.play();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(emscriptenMainLoop, 0, false);
#elifdef OS_SWITCH
    consoleInit(NULL);

    while (appletMainLoop()) {
        renderEngine->update();
        keyInputHandler->processInput(renderEngine->getWindow(), renderEngine->deltaTime);
    }

    userAppExit();
#else
    while (renderEngine->update() && keyInputHandler->processInput(renderEngine->getWindow(), renderEngine->deltaTime));
#endif

    return 0;
}