#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>

class Shader
{
public:

	unsigned int ID = 0;

	Shader(std::string vertexSource, std::string fragmentSource);
	
	static Shader* CreateShaderFromPath(const char* vertexPath, const char* fragmentPath);

	void Use();

	void SetBool(const char* name, bool value) const;
	void SetInt(const char* name, int value) const;
	void SetFloat(const char* name, float value) const;
	void SetFloat3(const char* name, const glm::vec3& value) const;
	void SetFloat4(const char* name, const glm::vec4& value) const;
	void SetMat4(const char* name, const glm::mat4& value) const;

private:

	void CreateShader(const char* vertexSource, const char* fragmentSource);

};

