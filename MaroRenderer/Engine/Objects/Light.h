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

	inline const glm::mat4 GetModelMatrix() { return glm::scale(glm::translate(glm::mat4(1.0f), GetLocation()), glm::vec3(0.2f)); }


private:

	Shader* CreateShader();
	Model* CreateModel();

	glm::vec3 m_Position;

};