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
#include "vec3.hpp"
#include "vec4.hpp"
#include "fwd.hpp"


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
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec4(const std::string &name, glm::vec4 value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
};

#endif //WIZENGINE3D_SHADER_H
