//
// Created by william on 13/03/23.
//

#include "OSV/rendering/RenderEngine.h"

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

    shader = std::shared_ptr<osv::Shader>(new osv::Shader(ASSET("shaders/defaultVertex.vs"), ASSET("shaders/defaultFragment.fs")));

    renderEngine->setMainShader(shader);

    osv::Model model(ASSET("models/backpack/Survival_BackPack_2.fbx"));
    renderEngine->addModel(model); // TODO: use references to models instead, or only pass path and create model inside of addModel.

//    osv::Model vertexShape(vertices, nullptr, sizeof(vertices), 0,
//                           ASSET("shaders/defaultVertex.vs"), ASSET("shaders/defaultFragment.fs"),
//                           ASSET("gfx/jesus.jpg"));

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