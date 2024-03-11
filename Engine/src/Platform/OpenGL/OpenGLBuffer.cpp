#include "OpenGLBuffer.h"

#include "OpenGLTypes.hpp"

#include "Renderer/Vertex.h"

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
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
		GLint alignment;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);

		const GLintptr alignedOffset = static_cast<GLintptr>(offset) / alignment * alignment;

		glBindBufferRange(Convert(p_target), binding, p_id, alignedOffset, static_cast<GLsizeiptr>(size));
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
