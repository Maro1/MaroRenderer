#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;
	InitMesh();
}

void Mesh::Draw(Shader* shader)
{
	shader->Use();

	shader->SetInt("diffuseMap", 0);
	shader->SetInt("normalMap", 0);

	shader->SetBool("usingDiffuse", false);
	shader->SetBool("usingNormal", false);

	//shader->SetInt("irradianceMap", GL_TEXTURE0 + m_Textures.size() + 1);

	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); 
		std::string number;
		TextureType type = m_Textures[i].Type;
		if (type == TextureType::DIFFUSE) 
		{
			shader->SetInt("diffuseMap", i);
			shader->SetBool("usingDiffuse", true);
		}
		else if (type == TextureType::NORMAL)
		{
			shader->SetInt("normalMap", i);
			shader->SetBool("usingNormal", true);
		}
		else if (type == TextureType::METALLIC)
		{
			shader->SetInt("metallicMap", i);
		}
		else if (type == TextureType::ROUGHNESS)
		{
			shader->SetInt("roughnessMap", i);
		}

		glBindTexture(GL_TEXTURE_2D, m_Textures[i].Id);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::AddTexture(unsigned int textureID, TextureType type)
{
	bool foundTexture = false;
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		if (m_Textures[i].Type == type)
		{
			glDeleteTextures(1, &m_Textures[i].Id);
			m_Textures[i].Id = textureID;
			foundTexture = true;
		}
	}
	if (!foundTexture)
	{
		Texture tex;
		tex.Id = textureID;
		tex.Type = type;
		m_Textures.push_back(tex);
	}
}

void Mesh::InitMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	// Vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Normal position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// TexCoord position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// Tangent vector
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	// BiTangent vector
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	glBindVertexArray(0);
}
