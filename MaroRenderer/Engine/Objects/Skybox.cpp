#include "Skybox.h"
#include <iostream>

#include <stb_image.h>


#include "Engine/Objects/Actor.h"

Skybox::Skybox(Camera* camera, Renderer* renderer, std::string texturePath) : m_Camera(camera), m_TexturePath(texturePath)
{
	m_HdrTexture = SkyboxFromFile();
	m_Renderer = renderer;
	m_EquiCubeShader = Shader::CreateShaderFromPath("Engine/Shaders/cubemap.vs", "Engine/Shaders/equi_to_cubemap.fs");
	m_IrradianceShader = Shader::CreateShaderFromPath("Engine/Shaders/cubemap.vs", "Engine/Shaders/irradiance.fs");
	m_BackgroundShader = Shader::CreateShaderFromPath("Engine/Shaders/background.vs", "Engine/Shaders/background.fs");
	Init();
}

Skybox::~Skybox()
{
	delete m_EquiCubeShader;
	delete m_IrradianceShader;
	delete m_BackgroundShader;
	glDeleteTextures(1, &m_HdrTexture);
	glDeleteTextures(1, &m_Cubemap);
}

void Skybox::Draw()
{
	glDepthFunc(GL_LEQUAL);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Renderer->GetFramebuffer());
	
	m_BackgroundShader->Use();
	m_BackgroundShader->SetMat4("view", m_Camera->GetView());
	m_BackgroundShader->SetMat4("projection", m_Camera->GetProjection());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int Skybox::SkyboxFromFile()
{
	stbi_set_flip_vertically_on_load(true);
	unsigned int textureID;

	int width, height, nrComponents;

	unsigned char* data = stbi_load(m_TexturePath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << m_TexturePath << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Skybox::Init()
{
	m_CaptureRenderbuffer.SetDimensions(m_Camera->GetViewPortSize().x, m_Camera->GetViewPortSize().y);
	m_CaptureRenderbuffer.Bind();
	m_CaptureFramebuffer.Bind();

	m_CaptureFramebuffer.AttachRenderbuffer(m_CaptureRenderbuffer.GetId(), GL_DEPTH_ATTACHMENT);

	// Setup Cubemap texture
	glGenTextures(1, &m_Cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);

	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Setup cube data
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	// link vertex attributes
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// HDR to Cubemap
	m_EquiCubeShader->Use();
	m_EquiCubeShader->SetInt("Skybox", 0);
	m_EquiCubeShader->SetMat4("projection", m_CaptureProjection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_HdrTexture);

	glViewport(0, 0, 512, 512);
	m_CaptureFramebuffer.Bind();

	for (unsigned int i = 0; i < 6; ++i)
	{
		m_EquiCubeShader->SetMat4("view", m_CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_Cubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &m_IrradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, m_Cubemap);
	glBindRenderbuffer(GL_RENDERBUFFER, m_Cubemap);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	m_IrradianceShader->Use();
	m_IrradianceShader->SetInt("Skybox", 0);
	m_IrradianceShader->SetMat4("projection", m_CaptureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFramebuffer.GetId());

	for (unsigned int i = 0; i < 6; ++i)
	{
		m_IrradianceShader->SetMat4("view", m_CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
