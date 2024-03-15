#include "OpenGLAPI.h"

#include "OpenGLBuffer.h"
#include "Core/Logger.hpp"

#include "OpenGLTypes.hpp"

namespace CGEngine::OpenGL
{
	void OpenGLAPI::Draw(const VertexArray* vertexArrayPtr) const
	{
		if (vertexArrayPtr != nullptr)
		{
			const auto& vertexArray = dynamic_cast<const GLVertexArray*>(vertexArrayPtr);

			vertexArray->Bind();
			switch (vertexArray->GetDrawType())
			{
			case DrawType::NONE:
				{
					CG_WARN("Please select a draw type!");
					break;
				}
			case DrawType::DRAW_ARRAYS:
				{
					const auto& vertices = vertexArray->GetVertices();
					glDrawArrays(GL_TRIANGLES, static_cast<GLint>(vertices.offset), static_cast<GLsizei>(vertices.count));
					break;
				}
			case DrawType::DRAW_ELEMENTS:
				{
					const auto& indices  = vertexArray->GetIndices();
					glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.count), GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(indices.offset));
					break;
				}
			case DrawType::DRAW_ARRAYS_INSTANCED:
				{
				CG_WARN("Draw arrays instanced not implemented yet!");
					break;
				}
			case DrawType::DRAW_ELEMENTS_INSTANCED:
				{
					const auto& indices = vertexArray->GetIndices();
					glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(indices.count), GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(indices.offset), 10);
					break;
				}
			}
			vertexArray->Unbind();
		}
	}

	void OpenGLAPI::Enable(const APICapability capability) const
	{
		glEnable(Convert(capability));
	}

	void OpenGLAPI::Disable(const APICapability capability) const
	{
		glDisable(Convert(capability));
	}

	void OpenGLAPI::Clear(const BufferMask mask)
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

	void OpenGLAPI::SetDepthFunc(const DepthFunc func) const
	{
		glDepthFunc(Convert(func));
	}

	void OpenGLAPI::SetDrawMode(const PolygonMode mode) const
	{
		glPolygonMode(GL_FRONT_AND_BACK, Convert(mode));
	}

}
