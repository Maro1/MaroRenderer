#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <vector>
#include "Model.h"

class SceneNode
{
public:

	SceneNode(Shader* shader, glm::vec3 location = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), Model* model = NULL);

	~SceneNode(void);

	void AddChild(SceneNode* child);
	void Update();

	inline glm::vec3 GetLocation() const { return m_Location; }
	inline glm::vec3 GetScale() const { return m_Scale; }

	inline void SetLocation(glm::vec3 location) { m_Location = location; }
	inline void SetScale(glm::vec3 scale) { m_Scale = scale; }

	void Draw();

private:
	SceneNode* m_Parent;
	std::vector<SceneNode*> m_Children;

	glm::vec3 m_Location;
	glm::vec3 m_Scale;

	glm::mat4 m_WorldTransform;
	glm::mat4 m_Transform;

	Model* m_Model;
	Shader* m_Shader;
};