#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

class ArcballCamera
{
public:
	ArcballCamera(int screenWidth, int screenHeight);

	void LeftMousePressed(bool pressed);
	void MouseMoved(double x, double y);

	inline const glm::mat4 GetView() { return glm::lookAt(m_Location, m_Target, m_Up); }
	inline const glm::vec3 GetPosition() { return m_Location; }


private:

	int m_ScreenWidth, m_ScreenHeight;

	glm::vec3 m_Target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Location = glm::vec3(0.0f, 0.0f, -3.0f);
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec2 m_PrevPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 m_CurrPos = glm::vec3(0.0f, 0.0f, 0.0f);



	int mouseEvent = 0;

};