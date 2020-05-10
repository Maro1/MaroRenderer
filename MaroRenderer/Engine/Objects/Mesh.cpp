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

	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		std::cout << m_Textures.size() << std::endl;
		glActiveTexture(GL_TEXTURE0 + i); 
		std::string number;
		TextureType type = m_Textures[i].Type;
		if (type == TextureType::DIFFUSE) 
		{
			shader->SetFloat("diffuseMap", i);
			glActiveTexture(GL_TEXTURE0);
		}
		else if (type == TextureType::NORMAL)
		{
			shader->SetFloat("normalMap", i);
			glActiveTexture(GL_TEXTURE1);
		}

		glBindTexture(GL_TEXTURE_2D, m_Textures[i].Id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
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
	glad_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
