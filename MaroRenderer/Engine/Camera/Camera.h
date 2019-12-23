#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

class Camera {
public:

	Camera(glm::vec3 position, bool enablePerspective, glm::vec2 screenSize);

	const inline glm::vec3 GetPosition() { return m_Position; }
	const inline glm::vec3 GetRotation() { return m_Rotation; }
	const inline glm::mat4 GetProjection() { return m_Projection; }
	const inline glm::mat4 GetView() { return m_View; }

private:

	int m_ScreenWidth, m_ScreenHeight;

	glm::mat4 m_Projection;
	glm::mat4 m_View;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;

	bool bPerspective;

	void Init();

};