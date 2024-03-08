#include "OpenGLAPI.h"
#include "Renderer/RenderAPI.h"

#include <glad/glad.h>

#include "OpenGLDrawObject.h"

namespace CGEngine::OpenGL
{
	constexpr uint32_t Convert(const ClearMask mask)
	{
		switch (mask)
		{
		case ClearMask::CG_NULL_BUFFER_BIT: break;
		case ClearMask::CG_COLOR_BUFFER_BIT:	   return GL_COLOR_BUFFER_BIT;
		case ClearMask::CG_DEPTH_BUFFER_BIT:       return GL_DEPTH_BUFFER_BIT;
		case ClearMask::CG_COLOR_DEPTH_BUFFER_BIT: return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		case ClearMask::CG_STENCIL_BUFFER_BIT:	   return GL_STENCIL_BUFFER_BIT;
		}

		return 0;
	}
	constexpr uint32_t Convert(const APICapability capability)
	{
		switch (capability)
		{
		case APICapability::CG_BLEND:			 return GL_BLEND;
		case APICapability::CG_CULL_FACE:		 return GL_CULL_FACE;
		case APICapability::CG_DEPTH_TEST:		 return GL_DEPTH_TEST;
		case APICapability::CG_FRAMEBUFFER_SRGB: return GL_FRAMEBUFFER_SRGB;
		case APICapability::CG_LINE_SMOOTH:		 return GL_LINE_SMOOTH;
		case APICapability::CG_POLYGON_SMOOTH:   return GL_POLYGON_SMOOTH;
		case APICapability::CG_SCISSOR_TEST:     return GL_SCISSOR_TEST;
		case APICapability::CG_STENCIL_TEST:     return GL_STENCIL_TEST;
		}

		return 0;
	}

	void OpenGLAPI::Draw(void* ptr) const
	{
		const auto& drawable_object = static_cast<GLDrawObject*>(ptr);

		//const auto& indices = vertex_array->GetIndices();
		//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.count), GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(indices.offset));
	}

	void OpenGLAPI::Enable(uint32_t capability) const
	{
		
	}

	void OpenGLAPI::Disable(uint32_t capability) const
	{
		
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
