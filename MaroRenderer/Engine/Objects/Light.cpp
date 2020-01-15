#include "Light.h"

Shader* Light::CreateShader()
{
	Shader* LightShader = new Shader(lightVShader, lightFShader);
	return LightShader;
}

Model* Light::CreateModel()
{
	std::string Path = "cube.obj";
	Model* LightModel = new Model(Path);
	return LightModel;
}

