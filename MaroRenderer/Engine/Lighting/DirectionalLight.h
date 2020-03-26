#pragma once

#include "Light.h"

class DirectionalLight : public SceneNode
{
public:

	DirectionalLight(glm::vec3 position);

	inline const glm::mat4 GetModelMatrix() { return glm::scale(glm::translate(glm::mat4(1.0f), GetLocation()), glm::vec3(0.2f)); }


private:


protected:
	glm::vec3 m_Position;

};