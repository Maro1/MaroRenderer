#include "Renderbuffer.h"

Renderbuffer::Renderbuffer()
{
	glGenRenderbuffers(1, &Id);
}

Renderbuffer::~Renderbuffer()
{
	glDeleteRenderbuffers(1, &Id);
}

void Renderbuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, Id);
}

void Renderbuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::SetDimensions(unsigned int width, unsigned int height)
{
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	Unbind();
}
