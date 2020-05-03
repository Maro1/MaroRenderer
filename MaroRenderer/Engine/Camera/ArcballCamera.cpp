#include "ArcballCamera.h"
#include <algorithm>

#include <iostream>

ArcballCamera::ArcballCamera(int screenWidth, int screenHeight)
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
}

void ArcballCamera::AltLeftMousePressed(bool pressed, int x, int y)
{
	if (!pressed)
	{
		m_AltMouseEvent = 0;
	}
	else
	{
		// Previous mouse position set when first clicking
		m_PrevPos = glm::vec2(x, y);
		m_AltMouseEvent = 1;
	}
}

void ArcballCamera::LeftMousePressed(bool pressed, int x, int y)
{
	if (!pressed)
	{
		m_MouseEvent = 0;
	}
	else
	{
		// Previous mouse position set when first clicking
		m_PrevPos = glm::vec2(x, y);
		m_MouseEvent = 1;
	}
}

void ArcballCamera::MouseMoved(double x, double y)
{

	if (m_AltMouseEvent == 1)
	{
		// Current mouse position
		m_CurrPos = glm::vec2(x, y);

		// Calculate camera vectors
		glm::vec3 targetToCamera(m_Location - m_Target);
		m_Right = glm::normalize(glm::cross(targetToCamera, m_Up));
		m_Up = glm::normalize(glm::cross(m_Right, targetToCamera));

		// Find new camera location
		m_Location = Tumble((m_PrevPos.x - m_CurrPos.x) * RotationSpeed, (m_CurrPos.y - m_PrevPos.y) * RotationSpeed);


		m_PrevPos = m_CurrPos;
	}

	/*if (m_MouseEvent == 1)
	{
		m_CurrPos = glm::vec2(x, y);

		m_Yaw += (x - m_PrevPos.x) * Sensitivity;
		m_Pitch += (m_PrevPos.y - y) * Sensitivity;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Yaw));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Up = glm::normalize(glm::cross(m_Right, direction));

		m_PrevPos = m_CurrPos;
	}*/
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

void ArcballCamera::Target(glm::vec3 target)
{
	glm::vec3 newPos = m_Location - (m_Target - target);
	SetTarget(target);
	SetLocation(newPos);
}

void ArcballCamera::Forward()
{
	m_Location += MoveSpeed * glm::cross(m_Right, m_Up);
	m_Target += MoveSpeed * glm::cross(m_Right, m_Up);
}

void ArcballCamera::Back()
{
	m_Location -= MoveSpeed * glm::cross(m_Right, m_Up);
	m_Target -= MoveSpeed * glm::cross(m_Right, m_Up);
}

void ArcballCamera::Left()
{
	m_Location += MoveSpeed * m_Right;
	m_Target += MoveSpeed * m_Right;
}

void ArcballCamera::Right()
{
	m_Location -= MoveSpeed * m_Right;
	m_Target -= MoveSpeed * m_Right;
}

glm::vec3 ArcballCamera::Tumble(float angleX, float angleY)
{
	// X rotation - Rodrigues Rotation Formula
	glm::vec3 originLoc = m_Location - m_Target;

	glm::vec3 NewLocation = (1 - glm::cos(angleX)) * (glm::dot(originLoc, m_Up)) * m_Up + glm::cos(angleX)
		* originLoc + glm::sin(angleX) * glm::cross(m_Up, originLoc);

	// Y rotation - Rodrigues Rotation Formula
	NewLocation = (1 - glm::cos(angleY)) * (glm::dot(NewLocation, m_Right)) * m_Right + glm::cos(angleY)
		* NewLocation + glm::sin(angleY) * glm::cross(m_Right, NewLocation);


	NewLocation += m_Target;

	return NewLocation;
}
