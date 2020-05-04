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
		m_AltPrevPos = glm::vec2(x, y);
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

void ArcballCamera::MouseMoved(double x, double y, bool alt)
{
	if (alt)
	{
		if (m_AltMouseEvent == 0)
			return;

		else
		{
			// Current mouse position
			m_AltCurrPos = glm::vec2(x, y);

			// Calculate camera vectors
			glm::vec3 targetToCamera(m_Location - m_Target);
			m_Right = glm::normalize(glm::cross(targetToCamera, m_Up));
			m_Up = glm::normalize(glm::cross(m_Right, targetToCamera));

			// Find new camera location
			m_Location = Tumble((m_AltPrevPos.x - m_AltCurrPos.x) * RotationSpeed, (m_AltCurrPos.y - m_AltPrevPos.y) * RotationSpeed);
		}
		m_AltPrevPos = m_AltCurrPos;
	}
	else
	{
		if (m_MouseEvent == 0)
			return;

		if (m_MouseEvent == 1)
		{ 
			/*Look around code*/
		}
		m_PrevPos = m_CurrPos;
	}
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

void ArcballCamera::Forward(float deltaTime)
{
	m_Location += MoveSpeed * glm::cross(m_Right, m_Up) * deltaTime;
	m_Target += MoveSpeed * glm::cross(m_Right, m_Up) * deltaTime;
}

void ArcballCamera::Back(float deltaTime)
{
	m_Location -= MoveSpeed * glm::cross(m_Right, m_Up) * deltaTime;
	m_Target -= MoveSpeed * glm::cross(m_Right, m_Up) * deltaTime;
}

void ArcballCamera::Left(float deltaTime)
{
	m_Location += MoveSpeed * m_Right * deltaTime;
	m_Target += MoveSpeed * m_Right * deltaTime;
}

void ArcballCamera::Right(float deltaTime)
{
	m_Location -= MoveSpeed * m_Right * deltaTime;
	m_Target -= MoveSpeed * m_Right * deltaTime;
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

glm::vec3 ArcballCamera::Look(float angleX, float angleY)
{
	// X rotation - Rodrigues Rotation Formula
	glm::vec3 originLoc = m_Target - m_Location;

	glm::vec3 NewTarget = (1 - glm::cos(angleX)) * (glm::dot(originLoc, m_Up)) * m_Up + glm::cos(angleX)
		* originLoc + glm::sin(angleX) * glm::cross(m_Up, originLoc);

	// Y rotation - Rodrigues Rotation Formula
	NewTarget = (1 - glm::cos(angleY)) * (glm::dot(NewTarget, m_Right)) * m_Right + glm::cos(angleY)
		* NewTarget + glm::sin(angleY) * glm::cross(m_Right, NewTarget);


	NewTarget += m_Location;

	return NewTarget;
}


