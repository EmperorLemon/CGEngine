#include "OpenGLAPI.h"
#include "Renderer/RenderAPI.h"

#include "OpenGLTypes.hpp"

#include "OpenGLBuffer.h"
#include "OpenGLDrawObject.h"
#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
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

	void OpenGLAPI::Enable(const APICapability capability) const
	{
		glEnable(Convert(capability));

		if (!glIsEnabled(Convert(capability))) CG_ERROR("Renderdoc was right!");
	}

	void OpenGLAPI::Disable(const APICapability capability) const
	{
		glDisable(Convert(capability));
	}

	void OpenGLAPI::Clear(const ClearMask mask)
	{
		glClear(Convert(mask));
	}

	void OpenGLAPI::ClearColor(const float* rgba)
	{
		glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
	}

	void OpenGLAPI::ResizeViewport(const int32_t x, const int32_t y, const int32_t width, const int32_t height)
	{
		glViewport(x, y, width, height);
	}
}
