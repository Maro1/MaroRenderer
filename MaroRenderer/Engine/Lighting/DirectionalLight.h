#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:

	DirectionalLight(Shader* shader, glm::vec3 position) : Light(position), m_Shader(shader)
	{
		InitLight();
	}

private:

	Shader* m_Shader;

	void InitLight();
};