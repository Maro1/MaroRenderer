#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

class Camera {
public:

	inline glm::mat4 GetView() const { return glm::lookAt(m_Location, m_Location + m_Front, m_Up); }
	inline glm::vec3 GetPosition() const { return m_Location; }
	inline glm::mat4 GetProjection() const {
		return glm::perspective(glm::radians(45.0f),
			(float)m_ViewPortWidth / (float)m_ViewPortHeight, 0.1f, 1000.0f);
	}

	void SetTarget(glm::vec3 target) { m_Target = target; }
	void SetLocation(glm::vec3 location) { m_Location = location; }
	void SetViewPortSize(int x, int y) { m_ViewPortWidth = x; m_ViewPortHeight = y; }
	glm::vec2 GetViewPortSize() { return glm::vec2(m_ViewPortWidth, m_ViewPortHeight); }

protected:

	int m_ViewPortWidth, m_ViewPortHeight;

	glm::vec3 m_Target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Location = glm::vec3(0.0f, 0.0f, -3.0f);
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, 1.0f);

};