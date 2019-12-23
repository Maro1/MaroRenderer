#include "ArcballCamera.h"
#include <algorithm>

#include <iostream>

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
		std::cout << "Location: X: " << x << " Y: " << y << std::endl;
		m_PrevPos = glm::vec2(x, y);
		mouseEvent = 2;
	}
	else {
		m_CurrPos = glm::vec2(x, y);
		glm::vec3 targetToCamera(m_Location - m_Target);
		glm::vec3 cameraRight(glm::normalize(glm::cross(targetToCamera, m_Up)));

		m_Up = glm::normalize(glm::cross(cameraRight, targetToCamera));

		m_Location = glm::rotate(m_Location, (m_CurrPos.y - m_PrevPos.y) * 0.01f, cameraRight) + m_Target;

		m_Location = glm::rotate(m_Location, (m_PrevPos.x - m_CurrPos.x) * 0.01f, m_Up) + m_Target;

	}
	m_PrevPos = m_CurrPos;
}
