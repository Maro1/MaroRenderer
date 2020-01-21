#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "Engine/Objects/SceneNode.h"
#include <string>

class Light : public SceneNode
{
public:

	Light(glm::vec3 position) : m_Position(position), SceneNode(CreateShader(), position, glm::vec3(2.0f), CreateModel()) { }

	inline const glm::vec3 GetPosition() { return m_Position; }
	inline const glm::mat4 GetModelMatrix() { return glm::scale(glm::translate(glm::mat4(1.0f), m_Position), glm::vec3(0.2f)); }

	inline void SetPosition(glm::vec3 position) { m_Position = position; }

private:

	Shader* CreateShader();
	Model* CreateModel();

	glm::vec3 m_Position;

};