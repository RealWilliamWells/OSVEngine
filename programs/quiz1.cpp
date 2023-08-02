//
// Created by william on 02/08/23.
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
    shader->setVec3("viewPos", osv::Mouse::camera->getPosition());
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

    // Add models
    osv::Model coorModel(shader, ASSET("models/coor_axis/coor_axis.dae"), false,
                         {0.f, 0.f, 0.f}, 0.f, {1.f, 1.f, 1.f}, {0.19f, 0.19f, 0.19f}, false);

    osv::Model netModel(shader, ASSET("models/tenis_net/tenis_net.dae"), false,
                        {0.f, 0.f, 0.f}, 0.f, {1.f, 1.f, 1.f}, {0.07f, 0.07f, 0.07f}, false);

    osv::Model sLetterModel(shader, ASSET("models/letters/s/s.dae"), false,
                            {25.f, 25.f, 15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f}, false);
    osv::Model racket1Model(shader, ASSET("models/tenis_racket/tenis_racket.dae"), false,
                            {25.f, 0.f, 15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f}, false);
    racket1Model.addChild(sLetterModel);

    osv::Model lLetterModel(shader, ASSET("models/letters/l/l.dae"), false,
                            {25.f, 25.f, -15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f}, false);
    osv::Model racket2Model(shader, ASSET("models/tenis_racket/tenis_racket.dae"), false,
                            {25.f, 0.f, -15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f}, false);
    racket2Model.addChild(lLetterModel);

    osv::Model wLetterModel(shader, ASSET("models/letters/w/w.dae"), false,
                            {-25.f, 25.f, 15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f}, false);
    osv::Model racket3Model(shader, ASSET("models/tenis_racket/tenis_racket.dae"), false,
                            {-25.f, 0.f, 15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f}, false);
    racket3Model.addChild(wLetterModel);

    osv::Model eLetterModel(shader, ASSET("models/letters/e/e.dae"), false,
                            {-25.f, 25.f, -15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f}, false);
    osv::Model racket4Model(shader, ASSET("models/tenis_racket/tenis_racket.dae"), false,
                            {-25.f, 0.f, -15.f}, 0.f, {1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f}, false);
    racket4Model.addChild(eLetterModel);

    osv::Model skyBoxModel(shader, ASSET("models/skybox/skybox.dae"), false,
                           {0.f, 0.f, 0.f}, 0.f, {1.f, 1.f, 1.f}, {6.f, 6.f, 6.f}, false);

    renderEngine->addModel(skyBoxModel);
    renderEngine->addModel(coorModel); // TODO: use references to models instead, or only pass path and create model inside of addModel.
    renderEngine->addModel(netModel);
    renderEngine->addModel(racket1Model);
    renderEngine->addModel(racket2Model);
    renderEngine->addModel(racket3Model);
    renderEngine->addModel(racket4Model);

    renderEngine->addDisplayGrid(shader);

    // Add lights
    osv::Light light({1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {.2f, .2f, .2f}, {0.f, 3.f, 0.f});

    renderEngine->addLight(light);

    // Input handling
    // TODO: remove this cringe
    osv::Mouse::camera = renderEngine->getCamera();

    keyInputHandler = std::shared_ptr<osv::KeyInputHandler>(new osv::KeyInputHandler(renderEngine));
    keyInputHandler->addBindings(osv::KeyBinds::generateWindowBinds());

    keyInputHandler->addSwitchingBindings(osv::KeyBinds::generateQuiz1Binds());
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
        shader->setVec3("viewPos", osv::Mouse::camera->getPosition());
    }

    userAppExit();
#else
    while (renderEngine->update() && keyInputHandler->processInput(renderEngine->getWindow(), renderEngine->deltaTime)) { shader->setVec3("viewPos", osv::Mouse::camera->getPosition());}
#endif

    return 0;
}