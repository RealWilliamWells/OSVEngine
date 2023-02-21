//
// Created by william on 25/01/23.
//

#ifndef WIZENGINE3D_SHADER_H
#define WIZENGINE3D_SHADER_H

#ifdef OS_SWITCH
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif
#include <string>

namespace osv {
    class Shader;
}

class osv::Shader {
public:
	unsigned int programID;

	Shader(const char* vertexPath, const char* fragmentPath);

    void compileAndLinkProgramShaders(const char *vShaderCode, const char *fShaderCode);

	void useShader();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif //WIZENGINE3D_SHADER_H
