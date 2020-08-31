#include "Skybox.h"
#include <iostream>

#include <stb_image.h>


#include "Engine/Objects/Actor.h"

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Skybox::Skybox(Camera* camera, Renderer* renderer, std::string texturePath) : m_Camera(camera), m_TexturePath(texturePath)
{
	m_HdrTexture = SkyboxFromFile();
	m_Renderer = renderer;
	m_EquiCubeShader = Shader::CreateShaderFromPath("Engine/Shaders/cubemap.vs", "Engine/Shaders/equi_to_cubemap.fs");
	m_IrradianceShader = Shader::CreateShaderFromPath("Engine/Shaders/cubemap.vs", "Engine/Shaders/irradiance.fs");
	m_BackgroundShader = Shader::CreateShaderFromPath("Engine/Shaders/background.vs", "Engine/Shaders/background.fs");
	m_PrefilterShader = Shader::CreateShaderFromPath("Engine/Shaders/cubemap.vs", "Engine/Shaders/prefilter.fs");
	m_BRDFShader = Shader::CreateShaderFromPath("Engine/Shaders/brdf.vs", "Engine/Shaders/brdf.fs");

	Init();
}

Skybox::~Skybox()
{
	delete m_EquiCubeShader;
	delete m_IrradianceShader;
	delete m_BackgroundShader;
	delete m_PrefilterShader;
	delete m_BRDFShader;
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_HdrTexture);

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

	// Setup quad data
	m_BRDFShader->Use();
	glGenVertexArrays(1, &Quad_VAO);
	glGenBuffers(1, &Quad_VBO);

	glBindVertexArray(Quad_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, Quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_QuadVertices), &m_QuadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

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

	// Generate mipmaps
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Setup irradiance map
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

	glViewport(0, 0, 32, 32); 
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFramebuffer.GetId());

	// Render irradiance map
	for (unsigned int i = 0; i < 6; i++)
	{
		m_IrradianceShader->SetMat4("view", m_CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Setup prefilter map
	glGenTextures(1, &m_PrefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);

	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Render prefilter map 
	m_PrefilterShader->Use();
	m_PrefilterShader->SetInt("Skybox", 0);
	m_PrefilterShader->SetMat4("projection", m_CaptureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);

	m_CaptureFramebuffer.Bind();
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		m_CaptureRenderbuffer.Bind();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		m_PrefilterShader->SetFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_PrefilterShader->SetMat4("view", m_CaptureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Generate BRDF LUT
	glGenTextures(1, &m_BRDFTexture);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, m_BRDFTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	m_CaptureFramebuffer.Bind();
	m_CaptureRenderbuffer.Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BRDFTexture, 0);

	glViewport(0, 0, 512, 512);
	m_BRDFShader->Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
