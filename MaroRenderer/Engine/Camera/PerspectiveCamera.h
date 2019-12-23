#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class PerspectiveCamera
{

public:

	PerspectiveCamera(glm::vec3 location);

	void SetLocation(glm::vec3 location) ;
	inline glm::vec3 GetLocation() { return m_Location; }
	inline glm::vec3 GetFront() { return m_Front; }
	inline glm::vec3 GetUp() { return m_Up; }

private:

	glm::vec3 m_Location;
	glm::vec3 m_Front;
	glm::vec3 m_Up;

};