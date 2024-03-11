#include "OpenGLAPI.h"
#include "Renderer/RenderAPI.h"

#include <glad/glad.h>

#include "OpenGLBuffer.h"
#include "OpenGLDrawObject.h"

namespace CGEngine::OpenGL
{
	constexpr GLenum Convert(const ClearMask mask)
	{
		switch (mask)
		{
		case ClearMask::NULL_BUFFER_BIT:		break;
		case ClearMask::COLOR_BUFFER_BIT:	    return GL_COLOR_BUFFER_BIT;
		case ClearMask::DEPTH_BUFFER_BIT:       return GL_DEPTH_BUFFER_BIT;
		case ClearMask::COLOR_DEPTH_BUFFER_BIT: return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		case ClearMask::STENCIL_BUFFER_BIT:	    return GL_STENCIL_BUFFER_BIT;
		}

		return 0;
	}
	constexpr GLenum Convert(const APICapability capability)
	{
		switch (capability)
		{
		case APICapability::BLEND:			  return GL_BLEND;
		case APICapability::CULL_FACE:		  return GL_CULL_FACE;
		case APICapability::DEPTH_TEST:		  return GL_DEPTH_TEST;
		case APICapability::STENCIL_TEST:     return GL_STENCIL_TEST;
		case APICapability::FRAMEBUFFER_SRGB: return GL_FRAMEBUFFER_SRGB;
		case APICapability::LINE_SMOOTH:	  return GL_LINE_SMOOTH;
		case APICapability::POLYGON_SMOOTH:   return GL_POLYGON_SMOOTH;
		case APICapability::SCISSOR_TEST:     return GL_SCISSOR_TEST;
		}

		return GL_DEPTH_TEST;
	}

	void OpenGLAPI::Draw(void* ptr) const
	{
		if (ptr != nullptr)
		{
			const auto& drawable_object = static_cast<GLDrawObject*>(ptr);

			for (const auto& vertexArray : drawable_object->GetVertexArrays())
			{
				const auto& indices = vertexArray.GetIndices();

				vertexArray.Bind();
				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.count), GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(indices.offset));
				vertexArray.Unbind();
			}
		}

	}

	void OpenGLAPI::Enable(uint32_t capability) const
	{
		glEnable(Convert(static_cast<APICapability>(capability)));
	}

	void OpenGLAPI::Disable(uint32_t capability) const
	{
		glDisable(Convert(static_cast<APICapability>(capability)));
	}

	void OpenGLAPI::Clear(const uint32_t mask)
	{
		glClear(Convert(static_cast<ClearMask>(mask)));
	}

	void OpenGLAPI::ClearColor(float* rgba)
	{
		glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
	}

	void OpenGLAPI::ResizeViewport(const int32_t x, const int32_t y, const int32_t width, const int32_t height)
	{
		glViewport(x, y, width, height);
	}
}
