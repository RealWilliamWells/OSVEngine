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

    shader = std::shared_ptr<osv::Shader>(new osv::Shader(ASSET("shaders/defaultVertex.vs"), ASSET("shaders/defaultFragment.fs")));

    renderEngine->setMainShader(shader);

    // Add models
    osv::Model coorModel(ASSET("models/coor_axis/coor_axis.dae"), false,
                         {0.f, 0.f, 0.f}, 0.f, {1.f, 1.f, 1.f}, {0.19f, 0.19f, 0.19f});

    osv::Model netModel(ASSET("models/tenis_net/tenis_net.dae"), false,
                         {0.f, 0.f, 0.f}, 0.f, {1.f, 1.f, 1.f}, {0.07f, 0.07f, 0.07f});

    osv::Model racket1Model(ASSET("models/tenis_racket/tenis_racket.dae"), false,
                        {25.f, 0.f, 15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f});

    osv::Model racket2Model(ASSET("models/tenis_racket/tenis_racket.dae"), false,
                           {25.f, 0.f, -15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f});

    osv::Model racket3Model(ASSET("models/tenis_racket/tenis_racket.dae"), false,
                           {-25.f, 0.f, 15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f});

    osv::Model racket4Model(ASSET("models/tenis_racket/tenis_racket.dae"), false,
                           {-25.f, 0.f, -15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f});

    osv::Model skyBoxModel(ASSET("models/skybox/skybox.dae"), false,
                        {0.f, 0.f, 0.f}, 0.f, {1.f, 1.f, 1.f}, {6.f, 6.f, 6.f});

    renderEngine->addModel(skyBoxModel);
    renderEngine->addModel(coorModel); // TODO: use references to models instead, or only pass path and create model inside of addModel.
    renderEngine->addModel(netModel);
    renderEngine->addModel(racket1Model);
    renderEngine->addModel(racket2Model);
    renderEngine->addModel(racket3Model);
    renderEngine->addModel(racket4Model);

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