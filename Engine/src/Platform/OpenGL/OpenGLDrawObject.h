#pragma once

#include <vector>
#include <memory>

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
		GLDrawObject() = default;
		GLDrawObject(std::vector<float>&& vertices, std::vector<uint16_t>&& indices, VertexLayout&& layout);
	private:
		std::shared_ptr<GLBuffer>	   m_vertexBuffer = nullptr;
		std::shared_ptr<GLVertexArray> m_vertexArray  = nullptr;
	};
}