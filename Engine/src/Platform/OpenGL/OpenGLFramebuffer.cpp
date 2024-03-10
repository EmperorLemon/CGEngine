#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	GLFramebuffer::GLFramebuffer() : Framebuffer()
	{
		CG_TRACE("Created GLFramebuffer!");

		glCreateFramebuffers(1, &p_id);
	}

	GLFramebuffer::~GLFramebuffer()
	{
		CG_TRACE("Deleted GLFramebuffer {0}", p_id);

		glDeleteFramebuffers(1, &p_id);
	}

	bool GLFramebuffer::CheckStatus(uint32_t target)
	{
		
	}


}