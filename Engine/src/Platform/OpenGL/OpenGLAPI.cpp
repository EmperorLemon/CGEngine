#include "OpenGLAPI.h"
#include "Renderer/RenderAPI.h"

#include <glad/glad.h>

#include "OpenGLBuffer.h"

namespace CGEngine::OpenGL
{
	constexpr uint32_t Convert(const ClearMask mask)
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

			//glDrawArrays(GL_TRIANGLES, vertex_array->GetVertexOffset(), vertex_array->GetVertexCount());

			const auto& indices = vertex_array->GetIndices();
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.count), GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(indices.offset));
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
