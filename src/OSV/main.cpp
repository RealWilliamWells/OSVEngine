//
// Created by william on 13/03/23.
//

#include "OSV/rendering/RenderEngine.h"
#include "Scene.h"

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

#ifdef __EMSCRIPTEN__
static void emscriptenMainLoop() {
    renderEngine->update();
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

    tbd::SceneManager sceneManager;

    std::stringstream input = sceneManager.importFromFile(ASSET("scenes/test.oscene"));

    std::shared_ptr<tbd::Scene> importedScene = sceneManager.importScene(input);

//    osv::Model vertexShape(vertices, nullptr, sizeof(vertices), 0,
//                           ASSET("shaders/defaultVertex.vs"), ASSET("shaders/defaultFragment.fs"),
//                           ASSET("gfx/jesus.jpg"));

    renderEngine->setScene(*importedScene, ASSET("shaders/defaultVertex.vs"), ASSET("shaders/defaultFragment.fs"));

//    Music backgroundMusic(ASSET("sfx/background.wav"));
//    backgroundMusic.play();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(emscriptenMainLoop, 0, false);
#elifdef OS_SWITCH
    consoleInit(NULL);

    while (appletMainLoop()) {
        renderEngine->update();
    }

    userAppExit();
#else
    while (renderEngine->update());
#endif

    return 0;
}