#pragma once

#include <vector>
#include <memory>

#include "Math/Vector3.h"

namespace CGEngine
{
	class VertexLayout;
}

namespace CGEngine::OpenGL
{
	class GLBuffer;
	class GLVertexArray;

	class GLDrawObject
	{
	public:
		GLDrawObject(std::vector<float>&& vertices, std::vector<uint16_t>&& indices, VertexLayout&& layout);

		Math::Vector3 position = Math::Vector3(0.0f);

		[[nodiscard]] const GLVertexArray& GetVertexArray() const;
	private:
		std::shared_ptr<GLBuffer>	   m_vertexBuffer = nullptr;
		std::shared_ptr<GLVertexArray> m_vertexArray  = nullptr;
	};
}