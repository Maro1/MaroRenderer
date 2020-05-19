#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <vector>
#include "Model.h"

class SceneNode
{
public:

	SceneNode(Shader* shader = NULL, glm::vec3 location = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), Model* model = NULL);

	~SceneNode(void);

	void AddChild(SceneNode* child);
	void Update();

	inline glm::vec3 GetLocation() const { return m_Location; }
	inline glm::vec3 GetLocationP() const { return m_Location; }
	inline glm::vec3 GetScale() const { return m_Scale; }
	inline glm::vec3 GetRotation() const { return m_Rotation; }

	inline void SetLocation(glm::vec3 location) { m_Location = location; }
	inline void SetScale(glm::vec3 scale) { m_Scale = scale; }
	inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
	inline void SetLabel(std::string& label) { m_Label = label; }

	inline Shader* GetShader() { return m_Shader; }
	inline std::vector<SceneNode*> GetChildren() { return m_Children; }
	inline Model* GetModel() { return m_Model; }
	glm::mat4 GetModelMatrix();
	inline unsigned int GetUUID() { return m_UUID; }
	inline std::string GetLabel() { return m_Label; }

	virtual bool HasMaterial() = 0;

	void Draw();

private:
	SceneNode* m_Parent;
	std::vector<SceneNode*> m_Children;

	std::string m_Label = "default";

	unsigned int m_UUID;
	static unsigned int Count() {
		static int count = 0;
		return count++;
	}

	glm::vec3 m_Location;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation = glm::vec3(0.0f);

	glm::mat4 m_WorldTransform;
	glm::mat4 m_Transform;

	Model* m_Model;
	Shader* m_Shader;
};