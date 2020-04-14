#include "SceneNode.h"

SceneNode::SceneNode(Shader* shader, glm::vec3 location, glm::vec3 scale, Model* model) : m_UUID(Count())
{
	m_Location = location;
	m_Scale = scale;
	m_Parent = nullptr;
	m_Scale = scale;
	m_Transform = glm::translate(glm::mat4(1.0f), m_Location);
	m_Model = model;
	m_Shader = shader;
	Update();
}

SceneNode::~SceneNode(void)
{
	for (auto c : m_Children)
	{
		delete c;
	}
	m_Children.clear();
}

void SceneNode::AddChild(SceneNode* child)
{
	m_Children.push_back(child);
	child->m_Parent = this;
}

void SceneNode::Update()
{
	if (m_Parent)
	{
		m_WorldTransform = m_Parent->m_WorldTransform * m_Transform;
	}
	else
	{
		m_WorldTransform = m_Transform;
	}
	for (auto child : m_Children)
	{
		child->Update();
	}
	Draw();
}

void SceneNode::Draw()
{
	if(m_Model) m_Model->Draw(m_Shader);
}
