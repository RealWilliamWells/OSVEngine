# OSVEngine
OpenGL scene viewer developed for COMP371 at Concordia University

## Nintendo Switch
### Build dependencies
`pacman -S switch-glfw switch-mesa switch-glm switch-glad`

### Building
`cmake -G "Unix Makefiles" --toolchain=DevkitA64Libnx.cmake -S . -B build`

### Launch on hardware with console output
`/opt/devkitpro/tools/bin/nxlink -s path/to/homebrew.nro`

## Webassembly
### Building
`emcmake cmake ..`
`make`

### Local Server Test
`python3 -m http.server`