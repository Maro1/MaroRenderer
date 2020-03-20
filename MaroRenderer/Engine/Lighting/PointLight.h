#pragma once

#include "Light.h"

class PointLight : public Light {
public:
	PointLight(Shader* shader, glm::vec3 position) : Light(position), m_Shader(shader), currentID(ID())
	{
		InitLight();
	}

private:
	glm::vec3 m_Constant;
	glm::vec3 m_Quadratic;
	glm::vec3 m_Linear;

	Shader* m_Shader;

	void InitLight();
	static int ID()
	{
		static int ID = 0;
		return ID++;
	}
	int currentID;
};
