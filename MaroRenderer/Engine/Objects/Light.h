#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "Engine/Objects/SceneNode.h"
#include "Engine/Objects/Model.h"

class Light : public SceneNode
{
public:

	Light(glm::vec3 position) : SceneNode(CreateShader(), position, glm::vec3(2.0f), CreateModel()) { }

	inline const glm::vec3 GetPosition() { return m_Position; }
	inline const glm::mat4 GetModel() { return glm::scale(glm::translate(glm::mat4(1.0f), m_Position), glm::vec3(0.2f)); }

	inline void SetPosition(glm::vec3 position) { m_Position = position; }

private:

	Shader* CreateShader();
	Model* CreateModel();

	glm::vec3 m_Position;

	const char* lightVShader = "#version 330 core\n"

		"layout(location = 0) in vec3 aPos;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main()\n"
		"{\n"
		"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"}\n";

	const char* lightFShader = "#version 330 core\n"

		"out vec4 color;\n"

		"void main()\n"
		"{\n"
		"color = vec4(1.0);\n"
		"}\n";

};