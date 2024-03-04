#include "OpenGLBuffer.h"

#include <glad/glad.h>

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	constexpr GLenum Convert(const DataType type)
	{
		switch (type)
		{
		case DataType::UNSIGNED_BYTE:  return GL_UNSIGNED_BYTE;
		case DataType::BYTE:		   return GL_BYTE;
		case DataType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
		case DataType::SHORT:		   return GL_SHORT;
		case DataType::UNSIGNED_INT:   return GL_UNSIGNED_INT;
		case DataType::INT:            return GL_INT;
		case DataType::FLOAT:	       return GL_FLOAT;
		}

		return GL_FLOAT;
	}

	GLBuffer::GLBuffer(const void* data, const size_t size, const int32_t count, const std::string_view name) : Buffer(size, count, 0)
	{
		glCreateBuffers(1, &p_id);
		glNamedBufferStorage(p_id, static_cast<GLsizeiptr>(size) * count, data, GL_DYNAMIC_STORAGE_BIT);

		if (!name.empty())
			glObjectLabel(GL_BUFFER, p_id, static_cast<GLsizei>(name.length()), name.data());
	}

	GLVertexArray::GLVertexArray(const GLBuffer* vertexBuffer, const GLBuffer* indexBuffer, const VertexLayout& layout) : VertexArray(0, 0, 0)
	{
		if (vertexBuffer == nullptr) CG_ERROR("Vertex buffer is nullptr!");

		glCreateVertexArrays(1, &p_id);

		const std::vector<VertexAttribute>& attributes = layout.GetAttributes();

		int32_t stride = 0;

		for (const auto& attribute : attributes)
			stride += attribute.count * GetSize(attribute.type);

		glVertexArrayVertexBuffer(p_id, 0, vertexBuffer->GetID(), 0, stride);
		if (indexBuffer != nullptr)  glVertexArrayElementBuffer(p_id, indexBuffer->GetID());

		for (const auto& attribute : attributes)
			glEnableVertexArrayAttrib(p_id, attribute.index);

		for (const auto& attribute : attributes)
			glVertexArrayAttribFormat(p_id, attribute.index, attribute.count, Convert(attribute.type), attribute.normalized, attribute.offset);

		for (const auto& attribute : attributes)
			glVertexArrayAttribBinding(p_id, attribute.index, 0);

		p_vertexCount = vertexBuffer->GetCount();
		if (indexBuffer != nullptr) p_indexCount = indexBuffer->GetCount();
	}

	GLVertexArray::~GLVertexArray()
	{
		glDeleteVertexArrays(1, &p_id);
	}

	void GLVertexArray::Bind() const
	{
		glBindVertexArray(p_id);
	}

	void GLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}
