cmake_minimum_required(VERSION 3.7)
project(OSVEngine)

set(STB_COMMIT 3ecc60f25ae1391cf6434578ece782afa1458b5)
set(GLM_COMMIT bf71a834948186f4097caa076cd2663c69a10e1e)
set(OPENAL_SOFT_COMMIT 70c14cd560db819f180073052f4c2bfae5cf3c31)
set(LIBSNDFILE_COMMIT master)
set(IMGUI_COMMIT f3f6295d532b98e322691afb8ef418209d963786) # Version 1.89.4
set(ASSIMP_COMMIT 9519a62dd20799c5493c638d1ef5a6f484e5faf1) # Version 5.2.5

set(GTEST_COMMIT e2239ee6043f73722e7aa812a459f54a28552929) # Version 1.11.0

if(CMAKE_COMPILER_IS_GNUCXX AND NOT WIN32)
    # Not for windows because windres.exe doesn't support those
    add_definitions(-Wall -ansi -Wno-deprecated -pthread -Wpedantic -fexceptions -Wno-dangling-reference -Wno-array-bounds -Wno-stringop-overflow)
endif()

# applies OS type C++ macro
if(WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lpthread -Wl,--no-whole-archive")

    add_compile_definitions(OS_WINDOWS)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_compile_definitions(OS_MAC)
elseif(UNIX)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,./lib/")

    add_compile_definitions(OS_UNIX)
endif()

#
# Set C++ version
#
set(CMAKE_CXX_STANDARD 23)

# Fetch dependencies
include(FetchContent)

FetchContent_Declare(stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG ${STB_COMMIT})
FetchContent_MakeAvailable(stb)
FetchContent_GetProperties(stb SOURCE_DIR STB_SOURCE_DIR)

FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG ${GLM_COMMIT})
FetchContent_MakeAvailable(glm)
FetchContent_GetProperties(glm SOURCE_DIR GLM_SOURCE_DIR)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten" AND NOT SWITCH)
    FetchContent_Declare(openal
            GIT_REPOSITORY https://github.com/kcat/openal-soft.git
            GIT_TAG ${OPENAL_SOFT_COMMIT})
    FetchContent_MakeAvailable(openal)
    FetchContent_GetProperties(openal SOURCE_DIR OPENAL_SOFT_SOURCE_DIR)
endif()

FetchContent_Declare(libsndfile
        GIT_REPOSITORY https://github.com/RealWilliamWells/libsndfile.git
        GIT_TAG ${LIBSNDFILE_COMMIT})
FetchContent_MakeAvailable(libsndfile)
FetchContent_GetProperties(libsndfile SOURCE_DIR LIBSNDFILE_SOURCE_DIR)

FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG ${IMGUI_COMMIT})
FetchContent_MakeAvailable(imgui)
FetchContent_GetProperties(imgui)
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_CUSTOM) # Indicate to IMGUI that it doesn't need to find its own OPENGL3 loader
add_compile_definitions(IMGUI_IMPL_OPENGL_ES3)

FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG ${ASSIMP_COMMIT})
FetchContent_MakeAvailable(assimp)
FetchContent_GetProperties(assimp SOURCE_DIR ASSIMP_SOURCE_DIR)
target_compile_options(assimp PRIVATE $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-frtti>) # Without this, cross platform builds fall (Switch)
SET(ASSIMP_BUILD_TESTS OFF)

# Other dependencies

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten" AND NOT SWITCH)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(Threads REQUIRED)
endif()

if (SWITCH)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
    include(SwitchTools)
    include_directories(${DEVKITPRO}/libnx/include/ ${DEVKITPRO}/portlibs/switch/include/)
endif()

# include and source files
include_directories(include ${STB_SOURCE_DIR} ${GLM_SOURCE_DIR}/glm ${OPENAL_SOFT_SOURCE_DIR}/include
        ${LIBSNDFILE_SOURCE_DIR}/include ${imgui_SOURCE_DIR} ${ASSIMP_SOURCE_DIR}/include)

file(GLOB SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/include/*.inl
        ${PROJECT_SOURCE_DIR}/include/*.h
        ${PROJECT_SOURCE_DIR}/include/*.hpp
        ${PROJECT_SOURCE_DIR}/src/*/*.cpp
        ${PROJECT_SOURCE_DIR}/include/*/*.inl
        ${PROJECT_SOURCE_DIR}/include/*/*.h
        ${PROJECT_SOURCE_DIR}/include/*/*.hpp
        ${PROJECT_SOURCE_DIR}/src/*/*/*.cpp
        ${PROJECT_SOURCE_DIR}/include/*/*/*.inl
        ${PROJECT_SOURCE_DIR}/include/*/*/*.h
        ${PROJECT_SOURCE_DIR}/include/*/*/*.hpp
        ${PROJECT_SOURCE_DIR}/src/*/*/*/*.cpp
        ${PROJECT_SOURCE_DIR}/include/*/*/*/*.inl
        ${PROJECT_SOURCE_DIR}/include/*/*/*/*.h
        ${PROJECT_SOURCE_DIR}/include/*/*/*/*.hpp

        ${imgui_SOURCE_DIR}/*.h
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)


if (NOT ${SWITCH})
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${GTEST_COMMIT})

    set(GTEST_FORCE_SHARED_CRT ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

#enable_testing()

#file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
#add_executable(${PROJECT_NAME}_tests ${TEST_SRC_FILES})

#target_link_libraries(${PROJECT_NAME}_tests
	#     gtest_main
	#     ${PROJECT_NAME})

#include(GoogleTest)
#gtest_discover_tests(${PROJECT_NAME}_tests)

add_executable(${PROJECT_NAME}_ass1 ${SOURCE_FILES} programs/assignment1.cpp)
add_executable(${PROJECT_NAME}_ass2 ${SOURCE_FILES} programs/assignment2.cpp)

add_executable(${PROJECT_NAME}_quiz1 ${SOURCE_FILES} programs/quiz1.cpp)
add_executable(${PROJECT_NAME}_quiz2 ${SOURCE_FILES} programs/quiz2.cpp)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(SWITCH)
    add_compile_definitions(OS_SWITCH)
    set_target_properties(${PROJECT_NAME}  PROPERTIES
            APP_TITLE "OSVEngine"
            APP_AUTHOR "William Wells"
            APP_VERSION "0.0.1"
            ROMFS res)
    target_link_libraries(${PROJECT_NAME}
            ${PORTLIBS}/lib/libglad.a
            ${PORTLIBS}/lib/libEGL.a
            ${PORTLIBS}/lib/libglfw3.a
            ${PORTLIBS}/lib/libdrm_nouveau.a
            ${PORTLIBS}/lib/libglapi.a
            ${PORTLIBS}/lib/libopenal.a
            ${PORTLIBS}/lib/libSDL2.a
            ${LIBNX}/lib/libnx.a
            sndfile
            assimp)

    add_nro_target(${PROJECT_NAME})
endif()

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten" AND NOT SWITCH)
    target_link_libraries(${PROJECT_NAME}_ass1 OpenGL::GL GLEW::glew glfw openal sndfile assimp)
    target_link_libraries(${PROJECT_NAME}_ass2 OpenGL::GL GLEW::glew glfw openal sndfile assimp)

    target_link_libraries(${PROJECT_NAME}_quiz1 OpenGL::GL GLEW::glew glfw openal sndfile assimp)
    target_link_libraries(${PROJECT_NAME}_quiz2 OpenGL::GL GLEW::glew glfw openal sndfile assimp)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    target_link_libraries(${PROJECT_NAME}_ass1 ${OPENGL_gl_LIBRARY} sndfile assimp)
    set_target_properties(${PROJECT_NAME}_ass1
            PROPERTIES SUFFIX ".html"
            LINK_FLAGS "-Os -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s WASM=1 -s TOTAL_MEMORY=128MB -s SAFE_HEAP=1 --preload-file res")

    target_link_libraries(${PROJECT_NAME}_ass2 ${OPENGL_gl_LIBRARY} sndfile assimp)
    set_target_properties(${PROJECT_NAME}_ass2
            PROPERTIES SUFFIX ".html"
            LINK_FLAGS "-Os -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s WASM=1 -s TOTAL_MEMORY=128MB -s SAFE_HEAP=1 --preload-file res")

    em_link_js_library(wasm ${libraryJsFiles})
endif()
