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
	void MouseMoved(double x, double y, bool alt, float deltaTime);
	void MouseScrolled(int offset);
	void Target(glm::vec3 target);

	void Forward(float deltaTime);
	void Back(float deltaTime);
	void Left(float deltaTime);
	void Right(float deltaTime);
	void Up(float deltaTime);
	void Down(float deltaTime);

private:

	int m_AltMouseEvent = 0;
	int m_MouseEvent = 0;

	glm::vec2 m_AltPrevPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 m_AltCurrPos = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec2 m_PrevPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 m_CurrPos = glm::vec3(0.0f, 0.0f, 0.0f);

	float m_Pitch = 0.0f;
	float m_Yaw = -90.0f;

	const float RotationSpeed = 0.05f;
	const float ScrollSpeed = 0.1f;
	const float MoveSpeed = 0.1f;
	const float Sensitivity = 0.005f;
	const int DefaultTargetDistance = 3;

	glm::vec3 Tumble(float angleX, float angleY);
	glm::vec3 Look(float angleX, float angleY);
};