# WizEngine3D
OpenGL game engine developed by the WWW team

## Webassembly
### Building
`emcmake cmake ..`
`make`

## Nintendo Switch
### Build dependencies
`pacman -S switch-glfw switch-mesa switch-glm switch-glad`

### Building
`cmake -G "Unix Makefiles" --toolchain=DevkitA64Libnx.cmake -S . -B build`

### Local Server Test
`python3 -m http.server`