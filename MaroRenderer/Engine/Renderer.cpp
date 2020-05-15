#include "Renderer.h"
#include "Logging/Logger.h"
#include "GLFW/glfw3.h"

Renderer::Renderer(Window* window)
{
	m_Window = window;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
	}
	glEnable(GL_DEPTH_TEST);

}

void Renderer::Clear()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::InitFramebuffer()
{
	glGenFramebuffers(1, &m_Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	glGenTextures(1, &m_FBOTexture);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Window->GetWidth(), m_Window->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Window->GetWidth(), m_Window->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
}

void Renderer::UpdateFrameBuffer(int x, int y)
{
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

void Renderer::StartRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void Renderer::StopRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Clear();
}

