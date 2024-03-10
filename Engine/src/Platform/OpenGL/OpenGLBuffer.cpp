#include "OpenGLBuffer.h"

#include <glad/glad.h>

#include "Renderer/Vertex.h"

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	constexpr GLenum Convert(const DataType type)
	{
		switch (type)
		{
		case DataType::VOID:		  
		case DataType::UNSIGNED_BYTE:  return GL_UNSIGNED_BYTE;
		case DataType::BYTE:		   return GL_BYTE;
		case DataType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
		case DataType::SHORT:		   return GL_SHORT;
		case DataType::UNSIGNED_INT:   return GL_UNSIGNED_INT;
		case DataType::INT:            return GL_INT;
		case DataType::FLOAT:	       return GL_FLOAT;
		case DataType::DOUBLE:		   return GL_DOUBLE;
		}

		return GL_FLOAT;
	}

	constexpr GLenum Convert(const BufferTarget target)
	{
		switch (target)
		{
		case BufferTarget::NONE: break;
		case BufferTarget::VERTEX_BUFFER:		  return GL_ARRAY_BUFFER;
		case BufferTarget::UNIFORM_BUFFER:		  return GL_UNIFORM_BUFFER;
		case BufferTarget::SHADER_STORAGE_BUFFER: return GL_SHADER_STORAGE_BUFFER;
		}

		return GL_UNIFORM_BUFFER;
	}

	GLBuffer::GLBuffer(const BufferTarget target, const size_t size, const void* data) : Buffer(target)
	{
		glCreateBuffers(1, &p_id);
		glNamedBufferStorage(p_id, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_STORAGE_BIT);
	}

	GLBuffer::~GLBuffer()
	{
		CG_TRACE("Deleted GLBuffer {0}", p_id);
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

	void GLBuffer::BindBufferBase(const uint32_t binding) const
	{
		glBindBufferBase(Convert(p_target), binding, p_id);
	}

	void GLBuffer::BindBufferRange(const uint32_t binding, const size_t offset, const size_t size) const
	{
		glBindBufferRange(Convert(p_target), binding, p_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size));
	}

	GLVertexArray::GLVertexArray(const uint32_t bufferID, const BufferInfo& vertexBuffer, const BufferInfo* indexBuffer, const VertexLayout& layout) : VertexArray(vertexBuffer)
	{
		glCreateVertexArrays(1, &p_id);

		const std::vector<VertexAttribute>& attributes = layout.GetAttributes();

		glVertexArrayVertexBuffer(p_id, 0, bufferID, static_cast<GLintptr>(vertexBuffer.offset), layout.GetStride());
		if (indexBuffer != nullptr) glVertexArrayElementBuffer(p_id, bufferID);

		for (const auto& attribute : attributes)
			glEnableVertexArrayAttrib(p_id, attribute.index);

		for (const auto& attribute : attributes)
			glVertexArrayAttribFormat(p_id, attribute.index, attribute.count, Convert(attribute.type), attribute.normalized, attribute.offset);

		for (const auto& attribute : attributes)
			glVertexArrayAttribBinding(p_id, attribute.index, 0);

		if (indexBuffer != nullptr)
			p_indexBuffer = std::make_optional<BufferInfo>(*indexBuffer);
	}

	GLVertexArray::~GLVertexArray()
	{
		CG_TRACE("Deleted GLVertexArray {0}", p_id);
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
