#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "Camera.h"

class ArcballCamera : public Camera
{
public:
	ArcballCamera(int screenWidth, int screenHeight);

	void LeftMousePressed(bool pressed, int x, int y);
	void MouseMoved(double x, double y);
	void MouseScrolled(int offset);


private:

	int mouseEvent = 0;

	glm::vec2 m_PrevPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 m_CurrPos = glm::vec3(0.0f, 0.0f, 0.0f);

	const float RotationSpeed = 0.01f;
	const float ScrollSpeed = 0.1f;

	glm::vec3 Tumble(float angleX, float angleY);
};