//
// Created by william on 29/01/23.
//

#include "OSV/rendering/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "gtc/type_ptr.hpp"

osv::Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // TODO: use future asset loader for loading GLSL code
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
#ifndef OS_SWITCH
    try {
#endif
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        std::string openGLVersionString = "#version 410";
#ifdef __EMSCRIPTEN__
        openGLVersionString = "#version 300 es";
#endif
        vShaderStream << openGLVersionString << "\n" << vShaderFile.rdbuf();
        fShaderStream << openGLVersionString << "\n" << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
#ifndef OS_SWITCH
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << std::endl;
    }
#endif
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    compileAndLinkProgramShaders(vShaderCode, fShaderCode);
}

void osv::Shader::compileAndLinkProgramShaders(const char *vShaderCode, const char *fShaderCode) {
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar**)&fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    useShader();
}

void osv::Shader::useShader() {
    glUseProgram(programID);
}

void osv::Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void osv::Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void osv::Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void osv::Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
}

void osv::Shader::setVec4(const std::string &name, glm::vec4 value) const {
    glUniform4f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z, value.w);
}
