#include "ArcballCamera.h"
#include <algorithm>

#include <iostream>

ArcballCamera::ArcballCamera(int screenWidth, int screenHeight)
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
}

void ArcballCamera::LeftMousePressed(bool pressed, int x, int y)
{
	if (!pressed)
	{
		mouseEvent = 0;
	}
	else
	{
		// Previous mouse position set when first clicking
		m_PrevPos = glm::vec2(x, y);
		mouseEvent = 1;
	}
}

void ArcballCamera::MouseMoved(double x, double y)
{
	if (mouseEvent == 0)
		return;

	else 
	{
		// Current mouse position
		m_CurrPos = glm::vec2(x, y);

		// Calculate camera vectors
		glm::vec3 targetToCamera(m_Location - m_Target);
		m_Right = glm::normalize(glm::cross(targetToCamera, m_Up));
		m_Up = glm::normalize(glm::cross(m_Right, targetToCamera));

		// Find new camera location
		m_Location = Tumble((m_PrevPos.x - m_CurrPos.x) * RotationSpeed, (m_CurrPos.y - m_PrevPos.y) * RotationSpeed);
	}
	m_PrevPos = m_CurrPos;
}

void ArcballCamera::MouseScrolled(int offset)
{
	if (offset < 0)
	{
		// Zoom out
		m_Location = ((float)(ScrollSpeed - offset)) * m_Location;
	}
	else
	{
		// Zoom in
		m_Location = ((float)(offset - ScrollSpeed)) * m_Location;
	}
}

glm::vec3 ArcballCamera::Tumble(float angleX, float angleY)
{
	// X rotation - Rodrigues Rotation Formula
	glm::vec3 NewLocation = (1 - glm::cos(angleX)) * (glm::dot(m_Location, m_Up)) * m_Up + glm::cos(angleX)
		* m_Location + glm::sin(angleX) * glm::cross(m_Up, m_Location);

	// Y rotation - Rodrigues Rotation Formula
	NewLocation = (1 - glm::cos(angleY)) * (glm::dot(NewLocation, m_Right)) * m_Right + glm::cos(angleY)
		* NewLocation + glm::sin(angleY) * glm::cross(m_Right, m_Location);

	return NewLocation;
}
