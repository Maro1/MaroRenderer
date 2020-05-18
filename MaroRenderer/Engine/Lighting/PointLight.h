#pragma once

#include "Light.h"

class PointLight : public Light {
public:
	PointLight(Shader* shader, glm::vec3 position) : Light(position), m_Shader(shader), currentID(ID())
	{
		m_Position = position;
		std::string label = "Point light " + std::to_string(currentID);
		SetLabel(label);
		InitLight();
	}

	~PointLight() {}

	inline int GetID() { return currentID; }
	inline float GetConstant() { return m_Constant; }
	inline float GetQuadradic() { return m_Quadratic; }
	inline float GetLinear() { return m_Linear; }
	inline glm::vec3 GetPosition() { return m_Position; }

	inline void SetPosition(glm::vec3 position) { m_Position = position; }

	virtual bool HasMaterial() override { return false; }

private:

	float m_Constant;
	float m_Quadratic;
	float m_Linear;

	glm::vec3 m_Position;

	Shader* m_Shader;

	void InitLight();
	static int ID()
	{
		static int ID = 0;
		return ID++;
	}
	int currentID;
};
