#include "Framebuffer.h"

Framebuffer::Framebuffer(Framebuffer* inputFramebuffer)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenFramebuffers(1, &Id);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &Id);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, Id);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::AttachTexture(unsigned int textureId, GLenum attachmentType, GLenum textureTarget)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget, textureId, 0);
}

void Framebuffer::AttachRenderbuffer(unsigned int renderbufferId, GLenum attachmentType)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, renderbufferId);
}
