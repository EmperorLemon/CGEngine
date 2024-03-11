#include "OpenGLFramebuffer.h"

#include "OpenGLTypes.hpp"

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	GLFramebuffer::GLFramebuffer(const BufferTarget target) : Framebuffer(target)
	{
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

	void GLFramebuffer::AttachRenderbuffer(const FramebufferTextureAttachment attachment, const uint32_t renderbuffer) const
	{
		glNamedFramebufferRenderbuffer(p_id, Convert(attachment), GL_RENDERBUFFER, renderbuffer);
	}

	void GLFramebuffer::Bind(const FramebufferTarget target) const
	{
		glBindFramebuffer(Convert(target), p_id);
	}

	void GLFramebuffer::Unbind(const FramebufferTarget target) const
	{
		glBindFramebuffer(Convert(target), 0);
	}

	void GLFramebuffer::Blit(const Math::IVector4& src, const Math::IVector4& dst, const BufferMask mask, const TextureFilter filter) const
	{
		glBlitNamedFramebuffer(p_id, 0, src[0], src[1], src[2], src[3], dst[0], dst[1], dst[2], dst[3], Convert(mask), Convert(filter));
	}

	void GLFramebuffer::Clear(const BufferType type, const int32_t index, const float* value) const
	{
		glClearNamedFramebufferfv(p_id, Convert(type), index, value);
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