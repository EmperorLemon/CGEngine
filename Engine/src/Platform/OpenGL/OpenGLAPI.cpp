#include "OpenGLAPI.h"
#include "Renderer/RenderAPI.h"

#include <glad/glad.h>

namespace CGEngine::OpenGL
{
	uint32_t Convert(const ClearMask mask)
	{
		switch (mask)
		{
		case ClearMask::CG_NULL_BUFFER_BIT:
			break;
		case ClearMask::CG_COLOR_BUFFER_BIT:
			return GL_COLOR_BUFFER_BIT;
		case ClearMask::CG_DEPTH_BUFFER_BIT:
			return GL_DEPTH_BUFFER_BIT;
		case ClearMask::CG_COLOR_DEPTH_BUFFER_BIT:
			return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		case ClearMask::CG_STENCIL_BUFFER_BIT:
			return GL_STENCIL_BUFFER_BIT;
		}

		return 0;
	}

	void OpenGLAPI::Clear(const uint32_t mask)
	{
		glClear(Convert(static_cast<ClearMask>(mask)));
	}

	void OpenGLAPI::ClearColor(float* rgba)
	{
		glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
	}
}