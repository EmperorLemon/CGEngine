#include "OpenGLDrawObject.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace CGEngine::OpenGL
{
	GLDrawObject::GLDrawObject(std::vector<float>&& vertices, std::vector<uint16_t>&& indices, VertexLayout&& layout)
	{
		const auto vBufferSize = sizeof(float) * vertices.size();
		const auto iBufferSize = sizeof(uint16_t) * indices.size();
		const size_t bufferSize = vBufferSize + iBufferSize;

		const BufferInfo vBufferInfo = { vBufferSize, vertices.size(), 0 };
		const BufferInfo iBufferInfo = { iBufferSize, indices.size(),  vBufferInfo.size};

		m_vertexBuffer = std::make_shared<GLBuffer>(bufferSize, nullptr);
		m_vertexBuffer->SetSubData(vBufferInfo.offset, vBufferInfo.size, vertices.data());
		m_vertexBuffer->SetSubData(iBufferInfo.offset, iBufferInfo.size, indices.data());

		m_vertexArray = std::make_shared<GLVertexArray>(m_vertexBuffer->GetID(), vBufferInfo, &iBufferInfo, layout);
	}
}
