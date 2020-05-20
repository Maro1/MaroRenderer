#include "Cubemap.h"
#include <iostream>

#include <stb_image.h>

Cubemap::Cubemap(Camera* camera, std::vector<std::string> texturePaths) : m_Camera(camera), m_TexturePaths(texturePaths)
{
	m_TextureID = CubemapFromFile();
	m_Shader = Shader::CreateShaderFromPath("Engine/Shaders/cubemap.vs", "Engine/Shaders/cubemap.fs");
	Init();
}

Cubemap::~Cubemap()
{
	delete m_Shader;
	glDeleteTextures(1, &m_TextureID);
}

void Cubemap::Draw()
{
	glDepthFunc(GL_LEQUAL);
	m_Shader->Use();
	m_Shader->SetInt("cubemap", 0);
	m_Shader->SetMat4("model", m_Model);
	m_Shader->SetMat4("view", glm::mat4(glm::mat3(m_Camera->GetView())));
	m_Shader->SetMat4("projection", m_Camera->GetProjection());

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}

unsigned int Cubemap::CubemapFromFile()
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;

	unsigned char* data;

	for (unsigned int i = 0; i < m_TexturePaths.size(); i++)
	{
		data = stbi_load(m_TexturePaths[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << m_TexturePaths[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Cubemap::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
