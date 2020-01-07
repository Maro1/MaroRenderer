#include "SceneNode.h"

SceneNode::SceneNode(Shader* shader, glm::vec3 location, glm::vec3 scale, Mesh* mesh)
{
	m_Location = location;
	m_Scale = scale;
	m_Parent = nullptr;
	m_Scale = scale;
	m_Transform = glm::translate(glm::mat4(1.0f), m_Location);
	m_Mesh = mesh;
	m_Shader = shader;
	Update();
}

SceneNode::~SceneNode(void)
{
	for (unsigned int i = 0; i < m_Children.size(); i++)
	{
		delete m_Children[i];
	}
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
	m_Mesh->Draw(m_Shader);
}
