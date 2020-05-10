#include "Light.h"

Shader* Light::CreateShader()
{
	std::string lightVShader = "#version 330 core\n"

		"layout(location = 0) in vec3 aPos;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main()\n"
		"{\n"
		"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"}\n";

	std::string lightFShader = "#version 330 core\n"

		"out vec4 color;\n"

		"void main()\n"
		"{\n"
		"color = vec4(1.0);\n"
		"}\n";
	return new Shader(std::string(lightVShader), std::string(lightFShader));
}

Model* Light::CreateModel()
{
	std::string Path = "Assets/cube.obj";
	Model* LightModel = new Model(Path);
	return LightModel;
}

