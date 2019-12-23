#include "Camera.h"

Camera::Camera(glm::vec3 position, bool enablePerspective, glm::vec2 screenSize)
{
	m_Position = position;
	bPerspective = enablePerspective;
	Init();
}

void Camera::Init()
{
	if (bPerspective)
	{
		m_Projection = glm::perspective(glm::radians(45.0f), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 100.0f);
	}
	else
	{
		m_Projection = glm::ortho(-m_ScreenWidth / 2, m_ScreenWidth / 2, -m_ScreenHeight / 2, m_ScreenHeight / 2);
	}
}
