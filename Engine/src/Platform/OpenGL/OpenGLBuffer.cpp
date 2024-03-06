#include "OpenGLBuffer.h"

#include <glad/glad.h>

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	constexpr GLenum Convert(const DataType type)
	{
		switch (type)
		{
		case DataType::VOID:		  
		case DataType::UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		case DataType::BYTE:		   return GL_BYTE;
		case DataType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
		case DataType::SHORT:		   return GL_SHORT;
		case DataType::UNSIGNED_INT:   return GL_UNSIGNED_INT;
		case DataType::INT:            return GL_INT;
		case DataType::FLOAT:	       return GL_FLOAT;
		}

		return GL_FLOAT;
	}

	GLBuffer::GLBuffer(const size_t size, const void* data) : Buffer(size)
	{
		glCreateBuffers(1, &p_id);
		glNamedBufferStorage(p_id, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_STORAGE_BIT);
	}

	GLBuffer::~GLBuffer()
	{
		CG_INFO("Deleted GLBuffer");
		glDeleteBuffers(1, &p_id);
	}

	void GLBuffer::SetData(const size_t size, const void* data) const
	{
		glNamedBufferData(p_id, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
	}

	void GLBuffer::SetSubData(const size_t offset, const size_t size, const void* data) const
	{
		glNamedBufferSubData(p_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
	}

	GLVertexArray::GLVertexArray(const GLBuffer* vertexBuffer, const VertexLayout& layout) : VertexArray(0, 0, 0)
	{
		if (vertexBuffer == nullptr) CG_ERROR("Vertex buffer is nullptr!");

		glCreateVertexArrays(1, &p_id);

		const std::vector<VertexAttribute>& attributes = layout.GetAttributes();

		int32_t stride = 0;

		for (const auto& attribute : attributes)
			stride += attribute.stride;

		glVertexArrayVertexBuffer(p_id, 0, vertexBuffer->GetID(), 0, stride);
		glVertexArrayElementBuffer(p_id, vertexBuffer->GetID());

		for (const auto& attribute : attributes)
			glEnableVertexArrayAttrib(p_id, attribute.index);

		for (const auto& attribute : attributes)
			glVertexArrayAttribFormat(p_id, attribute.index, attribute.count, GL_FLOAT, attribute.normalized, attribute.offset);

		for (const auto& attribute : attributes)
			glVertexArrayAttribBinding(p_id, attribute.index, 0);
	}

	GLVertexArray::~GLVertexArray()
	{
		CG_INFO("Deleted GLVertexArray");
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