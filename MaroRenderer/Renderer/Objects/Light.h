#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

class Light
{
public:

	Light(glm::vec3 position) : m_Position(position) { }

	inline const glm::vec3 GetPosition() { return m_Position; }
	inline const glm::mat4 GetModel() { return glm::scale(glm::translate(glm::mat4(1.0f), m_Position), glm::vec3(0.2f)); }

private:

	glm::vec3 m_Position;

};