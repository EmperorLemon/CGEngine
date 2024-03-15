#include "OpenGLFramebuffer.h"

#include "OpenGLTypes.hpp"

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	GLFramebuffer::GLFramebuffer(const BufferTarget target) : Framebuffer()
	{
		m_target = target;

		glCreateFramebuffers(1, &m_id);
	}

	GLFramebuffer::~GLFramebuffer()
	{
		CG_TRACE("Deleted GLFramebuffer {0}", m_id);

		glDeleteFramebuffers(1, &m_id);
	}

	void GLFramebuffer::AttachTexture(const FramebufferTextureAttachment attachment, const uint32_t texture) const
	{
		glNamedFramebufferTexture(m_id, Convert(attachment), texture, 0);
	}

	void GLFramebuffer::AttachRenderbuffer(const FramebufferTextureAttachment attachment, const uint32_t renderbuffer) const
	{
		glNamedFramebufferRenderbuffer(m_id, Convert(attachment), GL_RENDERBUFFER, renderbuffer);
	}

	void GLFramebuffer::Bind(const FramebufferTarget target) const
	{
		glBindFramebuffer(Convert(target), m_id);
	}

	void GLFramebuffer::Unbind(const FramebufferTarget target) const
	{
		glBindFramebuffer(Convert(target), 0);
	}

	void GLFramebuffer::Blit(const Math::IVector4& src, const Math::IVector4& dst, const BufferMask mask, const TParamValue filter) const
	{
		glBlitNamedFramebuffer(m_id, 0, src[0], src[1], src[2], src[3], dst[0], dst[1], dst[2], dst[3], Convert(mask), Convert(filter));
	}

	void GLFramebuffer::Clear(const BufferType type, const int32_t index, const float* value) const
	{
		glClearNamedFramebufferfv(m_id, Convert(type), index, value);
	}

	bool GLFramebuffer::CheckStatus() const
	{
		return glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	GLRenderbuffer::GLRenderbuffer(const BufferTarget target, const FramebufferTextureAttachmentFormat format, const int32_t width, const int32_t height) : Renderbuffer()
	{
		m_target = target;
		m_format = format;

		glCreateRenderbuffers(1, &m_id);
		glNamedRenderbufferStorage(m_id, Convert(format), width, height);
	}

	GLRenderbuffer::~GLRenderbuffer()
	{
		CG_TRACE("Deleted GLRenderbuffer {0}", m_id);

		glDeleteRenderbuffers(1, &m_id);
	}

	void GLRenderbuffer::ResizeBuffer(const int32_t width, const int32_t height) const
	{
		glNamedRenderbufferStorage(m_id, Convert(m_format), width, height);
	}
}