#include "ArcballCamera.h"
#include <algorithm>

ArcballCamera::ArcballCamera(int screenWidth, int screenHeight)
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
}

void ArcballCamera::LeftMousePressed(bool pressed)
{
	if (pressed == false) mouseEvent = 0;
	else
	{
		mouseEvent = 1;
	}
}

void ArcballCamera::MouseMoved(double x, double y)
{
	if (mouseEvent == 0)
		return;
	else if (mouseEvent == 1)
	{
		m_PrevPos = glm::vec2(x, y);
		mouseEvent = 2;
	}
	else {
		m_CurrPos = glm::vec2(x, y);
		glm::vec3 target_to_camera_vector(m_Location - m_Target);
		glm::vec3 camera_right_vector(glm::normalize(glm::cross(target_to_camera_vector, m_Up)));
		m_Up = glm::normalize(glm::cross(camera_right_vector, target_to_camera_vector));

		m_Location = (glm::rotate(m_Location, 0.0f, camera_right_vector) 
				* (m_Location - m_Target)) + m_Target;

		m_Location = (glm::rotate(m_Location, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f))
			* (m_Location - m_Target)) + m_Target;

	}
	m_PrevPos = m_CurrPos;
}
