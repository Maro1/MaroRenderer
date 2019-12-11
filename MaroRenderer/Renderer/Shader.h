#pragma once

class Shader
{
public:

	unsigned int ID = 0;

	Shader(const char* vertexSource, const char* fragmentSource);
	
	static Shader CreateShaderFromPath(const char* vertexPath, const char* fragmentPath);

	void Use();

private:

	void CreateShader(const char* vertexSource, const char* fragmentSource);

};

