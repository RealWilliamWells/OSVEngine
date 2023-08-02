# OSVEngine
OpenGL scene viewer developed for COMP371 at Concordia University

# For Markers
## Assignment 2
This project contains many different components. It takes advantage of libraries such as Assimp and stb (all linked below)  in order to import models that were custom made in Blender.

### Notable features
* Full coordinate system that properly localizes models in 3D space and allow for transformations.
* Easy interfacing with rendering engine and input handler, making it very easy to add models and other assets. Allowing for custom programs to be easily written using the pre-existing rendering code.
* Adaptable input handling allowing for easily changable controls. (still requires minor code edits to change controls)
* Ability to swap between control schems. The controls required for this assignment are used by default, but can be changed to freefly controls on the fly with hotkey Q
* Basic sound support via OpenAL.
* Full cross capability with WebGL via WebAssembly and Nintendo Switch via homebrew. Check out the WebGL version [here](http://wwells.net/)! (takes awhile to load and there is no loading bar implemented yet!)
* Joystick support. (except for WebGL unfortunately.
* Importation of many different 3D model assets formats. (Mainly .dae, aka COLLADA,  was tested)

### Controls
WSAD - Move model
ZX - Rotate model around Y-axis
SPACE - Move model to random position within grid
E - Change between models to move

Direction arrows - Change world orientation
HOME - Return world orientation to original position

P - Set rendering mode to points
L - Set rendering mode to lines
T - Set rendering mode to triangles

Left mouse button held - Zoom into/out of scene (camera moves forwards and backwards)
Right mouse button held - Pan camera
Middle mouse button held - Tilt camera

Left Control - Toggle mouse capture

ESC - Quit

Q - Toggle freefly mode (allows free movement of camera with mouse and WSAD control, disabling other controls)

### Project Structure Breakdown
### Rendering Engine
Almost everything pretaining to rendering related roles is located under "src/OSV/rendering/". The "glue" class that holds everything together is "RenderEngine.cpp" under that directory.

You can find the model, shader and mesh classes here. Model is the parent of a list of meshes and make up a complete model, while having parts that are able to move on their own. When a model is moved, all meshes that make it up are moved with it.

### Input Handler
The input handler is its module that uses the rendering engine interface to allow user control of the environment and window. It is located under "src/OSV/input/", the glue of the handler is "KeyInputHandler.cpp". This handler is also used for joystick control.

The actual bindings configurated for user control are located in "include/input/keyConfigs/keyBinds.h", alongside a "configStruct.h" file that defines some types for function callbacks and custom data structures.

Additionally the mouse input is covered by the callbacks located in "include/OSV/input/MouseInput.h". You can find the handling for the two different control schemes here as well.

### Assets and Shader Code
All assets can be located under "res/". The models and shader vertex and fragment codes used are all located here. The CMake script of the project copies these assets to the build directory and loads them into the asset files necessary in the case of the web and Nintendo Switch builds.

### Submission Program Entry Point
The driver for the rendering engine and input handler is "src/OSV/main.cpp". This uses the two drivers to produce the program for the assignment submissions.

### Major External Libraries
* GLEW for PC and WebGL builds, and Glad for Nintendo Switch build.
* [stb](https://github.com/nothings/stb) for image parsing for textures.
* [GLM](https://github.com/g-truc/glm) for matrices and vector math.
* [OpenAL](https://github.com/kcat/openal-soft) for audio playback.
* [libsndfile](https://github.com/RealWilliamWells/libsndfile) for parsing audio files.
* [imgui](https://github.com/nothings/stb) for future potiential GUI needs.
* [emscripten](https://emscripten.org/) for WebAssembly and WebGL APIs and build tools.
* [LibNX](https://github.com/switchbrew/libnx) for Nintendo Switch OS and hardware interfacing.

# Build Instructions
All dependencies except for GLEW and the frameworks and libraries for cross compilation are automatically fetched in the CMake scripts. This makes it exetremely portable but take a long time to run for the first time as it needs to clone and build the dependencies.

## PC
Make sure that you have GLEW installed and it is able to be located by your system.
`mkdir build && cd build`
`cmake ../ -B ./`
`make`

## Nintendo Switch
### Build dependencies
`pacman -S switch-glfw switch-mesa switch-glm switch-glad switch-openal-soft`

### Building
`cmake -G "Unix Makefiles" --toolchain=DevkitA64Libnx.cmake -S .. -B ./`
`make`

Cmake is currently not properly adding the romfs, so this must be done for now:
`/opt/devkitpro/tools/bin/elf2nro OSVEngine OSVCUSTOM.nro --nacp=OSVEngine.nacp --romfsdir=res`

### Launch on hardware with console output
`/opt/devkitpro/tools/bin/nxlink -s path/to/homebrew.nro`

## Webassembly
### Building
`emcmake cmake ..`
`make`

### Local Server Test
`python3 -m http.server`
