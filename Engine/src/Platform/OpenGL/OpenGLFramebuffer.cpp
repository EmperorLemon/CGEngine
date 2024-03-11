#include "OpenGLFramebuffer.h"

#include "OpenGLTypes.hpp"

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	GLFramebuffer::GLFramebuffer(const BufferTarget target) : Framebuffer(target)
	{
		CG_TRACE("Created GLFramebuffer!");

		glCreateFramebuffers(1, &p_id);
	}

	GLFramebuffer::~GLFramebuffer()
	{
		CG_TRACE("Deleted GLFramebuffer {0}", p_id);

		glDeleteFramebuffers(1, &p_id);
	}

	void GLFramebuffer::AttachTexture(const FramebufferTextureAttachment attachment, const uint32_t texture) const
	{
		glNamedFramebufferTexture(p_id, Convert(attachment), texture, 0);
	}

	bool GLFramebuffer::CheckStatus() const
	{
		return glCheckNamedFramebufferStatus(p_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	GLRenderbuffer::GLRenderbuffer(const BufferTarget target, const FramebufferTextureAttachmentFormat format, const int32_t width, const int32_t height) : Renderbuffer(target)
	{
		glCreateRenderbuffers(1, &p_id);
		glNamedRenderbufferStorage(p_id, Convert(format), width, height);
	}

	GLRenderbuffer::~GLRenderbuffer()
	{
		CG_TRACE("Deleted GLRenderbuffer {0}", p_id);

		glDeleteRenderbuffers(1, &p_id);
	}

}