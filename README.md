# OSVEngine
OpenGL scene viewer developed for COMP371 at Concordia University

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