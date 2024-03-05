#include "OpenGLAPI.h"
#include "Renderer/RenderAPI.h"

#include <glad/glad.h>

#include "OpenGLBuffer.h"

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

	void OpenGLAPI::Draw(void* array_ptr) const
	{
		if (array_ptr != nullptr)
		{
			const auto& vertex_array = static_cast<GLVertexArray*>(array_ptr);

			//glDrawArrays(GL_TRIANGLES, 0, vertex_array->GetVertexCount());
			glDrawElements(GL_TRIANGLES, vertex_array->GetIndexCount(), GL_UNSIGNED_SHORT, nullptr);
		}
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
