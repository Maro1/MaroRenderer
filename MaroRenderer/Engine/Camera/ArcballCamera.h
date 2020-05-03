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

	void AltLeftMousePressed(bool pressed, int x, int y);
	void LeftMousePressed(bool pressed, int x, int y);
	void MouseMoved(double x, double y);
	void MouseScrolled(int offset);
	void Target(glm::vec3 target);

	void Forward();
	void Back();
	void Left();
	void Right();

private:

	int m_AltMouseEvent = 0;
	int m_MouseEvent = 0;

	float m_Yaw = 0;
	float m_Pitch = 0;

	glm::vec2 m_PrevPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 m_CurrPos = glm::vec3(0.0f, 0.0f, 0.0f);

	const float RotationSpeed = 0.01f;
	const float ScrollSpeed = 0.1f;
	const float MoveSpeed = 0.01f;
	const float Sensitivity = 0.05f;

	glm::vec3 Tumble(float angleX, float angleY);
};