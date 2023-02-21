cmake_minimum_required(VERSION 3.7)
project(OSVEngine)

set(STB_COMMIT 3ecc60f25ae1391cf6434578ece782afa1458b5)
set(GLM_COMMIT bf71a834948186f4097caa076cd2663c69a10e1e)
set(GTEST_COMMIT e2239ee6043f73722e7aa812a459f54a28552929) # Version 1.11.0

if(CMAKE_COMPILER_IS_GNUCXX AND NOT WIN32)
    # Not for windows because windres.exe doesn't support those
    add_definitions(-frtti -Wall -ansi -Wno-deprecated -pthread -Wpedantic)
endif()

# applies OS type C++ macro
if(WIN32)
    #add_compile_definitions(OS_WINDOWS)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #add_compile_definitions(OS_MAC)
elseif(UNIX)
    #add_compile_definitions(OS_UNIX)
endif()

#
# Set C++ version
#
set(CMAKE_CXX_STANDARD 20)


#
# The following if-else chain makes the different platform output at different
# paths:
#
# bin/
#   windows64/
#   windows32/
#   mac/
#   linux64/
#   linux32/
#
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/windows64/)
    else()
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/windows32/)
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/mac/)
elseif(UNIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/linux64/)
    else()
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/linux32/)
    endif()
endif()

# Fetch dependencies
include(FetchContent)

FetchContent_Declare(stb
        GIT_REPOSITORY git@github.com:nothings/stb.git
        GIT_TAG ${STB_COMMIT})
FetchContent_MakeAvailable(stb)
FetchContent_GetProperties(stb SOURCE_DIR STB_SOURCE_DIR)

FetchContent_Declare(glm
        GIT_REPOSITORY git@github.com:g-truc/glm.git
        GIT_TAG ${GLM_COMMIT})
FetchContent_MakeAvailable(glm)
FetchContent_GetProperties(glm SOURCE_DIR GLM_SOURCE_DIR)

# Other dependencies

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(Threads REQUIRED)
endif()

# include and source files
include_directories(include ${STB_SOURCE_DIR} ${GLM_SOURCE_DIR}/glm)

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
        ${PROJECT_SOURCE_DIR}/include/*/*/*.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG ${GTEST_COMMIT})

set(GTEST_FORCE_SHARED_CRT ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#enable_testing()

#file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
#add_executable(${PROJECT_NAME}_tests ${TEST_SRC_FILES})

#target_link_libraries(${PROJECT_NAME}_tests
	#     gtest_main
	#     ${PROJECT_NAME})

#include(GoogleTest)
#gtest_discover_tests(${PROJECT_NAME}_tests)

# TODO: remove the following below when test binary build is no longer needed
add_executable(test_${PROJECT_NAME} ${SOURCE_FILES})

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    target_link_libraries(test_${PROJECT_NAME} OpenGL::GL GLEW::glew glfw)
else()
    target_link_libraries(test_${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
    set_target_properties(test_${PROJECT_NAME}
            PROPERTIES SUFFIX ".html"
            LINK_FLAGS "-Os -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s WASM=1 -s TOTAL_MEMORY=128MB -s SAFE_HEAP=1 --preload-file res")
    em_link_js_library(wasm ${libraryJsFiles})
endif()

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
