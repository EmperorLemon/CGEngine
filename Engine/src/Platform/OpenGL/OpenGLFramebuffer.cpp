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

	bool GLFramebuffer::CheckStatus() const
	{
		return glCheckNamedFramebufferStatus(p_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}


}