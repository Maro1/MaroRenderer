#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(glm::vec3 location)
{
	m_Location = location;
	m_Front = glm::vec3(0.0f, 0.1f, -1.0f);
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void PerspectiveCamera::SetLocation(glm::vec3 location) 
{
	m_Location = location;
}

