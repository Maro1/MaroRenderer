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

void ArcballCamera::MouseMoved(double x, double y, bool alt, float deltaTime)
{
	if (alt)
	{
		if (m_AltMouseEvent == 0)
			return;

		else
		{
			// Current mouse position
			m_AltCurrPos = glm::vec2(x, y);

			// Find new camera location
			m_Location = Tumble((m_AltPrevPos.x - m_AltCurrPos.x) * RotationSpeed * deltaTime, (m_AltPrevPos.y - m_AltCurrPos.y) * RotationSpeed * deltaTime);

			// Calculate camera vectors
			m_Front = glm::normalize(m_Target - m_Location);
			glm::vec3 right = glm::normalize(glm::cross(m_Front, m_Up));
			m_Up = glm::normalize(glm::cross(right, m_Front));
		}
		m_AltPrevPos = m_AltCurrPos;
	}
	else
	{
		if (m_MouseEvent == 0)
			return;

		if (m_MouseEvent == 1)
		{ 
			m_CurrPos = glm::vec2(x, y);
			m_Front = Look((m_PrevPos.x - m_CurrPos.x) * Sensitivity * deltaTime, (m_CurrPos.y - m_PrevPos.y) * Sensitivity * deltaTime);

			glm::vec3 right = glm::normalize(glm::cross(m_Front, m_Up));
			m_Up = glm::normalize(glm::cross(right, m_Front));
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
	m_Location += MoveSpeed * m_Front * deltaTime;
	m_Target += MoveSpeed * m_Front * deltaTime;
}

void ArcballCamera::Back(float deltaTime)
{
	m_Location -= MoveSpeed * m_Front * deltaTime;
	m_Target -= MoveSpeed * m_Front * deltaTime;
}

void ArcballCamera::Left(float deltaTime)
{
	m_Location -= MoveSpeed * glm::normalize(glm::cross(m_Front, m_Up)) * deltaTime;
	m_Target -= MoveSpeed * glm::normalize(glm::cross(m_Front, m_Up)) * deltaTime;
}

void ArcballCamera::Right(float deltaTime)
{
	m_Location += MoveSpeed * glm::normalize(glm::cross(m_Front, m_Up)) * deltaTime;
	m_Target += MoveSpeed * glm::normalize(glm::cross(m_Front, m_Up)) * deltaTime;
}

void ArcballCamera::Up(float deltaTime)
{
	m_Location += MoveSpeed * m_Up * deltaTime;
	m_Target += MoveSpeed * m_Up * deltaTime;
}

void ArcballCamera::Down(float deltaTime)
{
	m_Location -= MoveSpeed * m_Up * deltaTime;
	m_Target -= MoveSpeed * m_Up * deltaTime;
}

glm::vec3 ArcballCamera::Tumble(float angleX, float angleY)
{
	// X rotation - Rodrigues Rotation Formula
	glm::vec3 originLoc = m_Location - m_Target;

	glm::vec3 NewLocation = (1 - glm::cos(angleX)) * (glm::dot(originLoc, m_Up)) * m_Up + glm::cos(angleX)
		* originLoc + glm::sin(angleX) * glm::cross(m_Up, originLoc);

	// Y rotation - Rodrigues Rotation Formula
	NewLocation = (1 - glm::cos(angleY)) * (glm::dot(NewLocation, glm::normalize(glm::cross(m_Front, m_Up)))) * glm::normalize(glm::cross(m_Front, m_Up)) + glm::cos(angleY)
		* NewLocation + glm::sin(angleY) * glm::cross(glm::normalize(glm::cross(m_Front, m_Up)), NewLocation);


	NewLocation += m_Target;

	return NewLocation;
}

glm::vec3 ArcballCamera::Look(float angleX, float angleY)
{
	glm::vec3 newFront1 = glm::rotate(m_Front, angleX, m_Up);
	glm::vec3 newFront2 = glm::rotate(newFront1, angleY, glm::normalize(glm::cross(m_Up, newFront1)));

	return newFront2;
}


