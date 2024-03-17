#include "OpenGLBuffer.h"

#include "OpenGLTypes.hpp"

#include "Renderer/Vertex.h"

#include "Core/Logger.hpp"

namespace CGEngine::OpenGL
{
	GLBuffer::GLBuffer(const BufferTarget target, const size_t size, const void* data) : Buffer()
	{
		m_target = target;

		glCreateBuffers(1, &m_id);
		glNamedBufferStorage(m_id, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_STORAGE_BIT);
	}

	GLBuffer::~GLBuffer()
	{
		//CG_TRACE("Deleted GLBuffer {0}", m_id);
		glDeleteBuffers(1, &m_id);
	}

	void GLBuffer::SetData(const size_t size, const void* data) const
	{
		glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
	}

	void GLBuffer::SetSubData(const size_t offset, const size_t size, const void* data) const
	{
		glNamedBufferSubData(m_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
	}

	void GLBuffer::BindBufferBase(const uint32_t binding) const
	{
		glBindBufferBase(Convert(m_target), binding, m_id);
	}

	void GLBuffer::BindBufferRange(const uint32_t binding, const size_t offset, const size_t size) const
	{
		if (m_target == BufferTarget::UNIFORM_BUFFER)
		{
			GLint alignment;
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);

			const GLintptr alignedOffset = static_cast<GLintptr>(offset) / alignment * alignment;
			glBindBufferRange(Convert(m_target), binding, m_id, alignedOffset, static_cast<GLsizeiptr>(size));
		}
		else
			glBindBufferRange(Convert(m_target), binding, m_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size));
	}

	GLVertexArray::GLVertexArray(const uint32_t bufferID, const BufferInfo& vertexBuffer, const BufferInfo* indexBuffer, const VertexLayout& layout) : VertexArray()
	{
		glCreateVertexArrays(1, &m_id);

		const std::vector<VertexAttribute>& attributes = layout.GetAttributes();

		glVertexArrayVertexBuffer(m_id, 0, bufferID, static_cast<GLintptr>(vertexBuffer.offset), layout.GetStride());
		if (indexBuffer != nullptr) glVertexArrayElementBuffer(m_id, bufferID);

		for (const auto& attribute : attributes)
			glEnableVertexArrayAttrib(m_id, attribute.index);

		for (const auto& attribute : attributes)
			glVertexArrayAttribFormat(m_id, attribute.index, attribute.count, Convert(attribute.type), attribute.normalized, attribute.offset);

		for (const auto& attribute : attributes)
			glVertexArrayAttribBinding(m_id, attribute.index, 0);

		m_vertexBuffer = vertexBuffer;
		if (indexBuffer != nullptr) m_indexBuffer = *indexBuffer;
	}

	GLVertexArray::~GLVertexArray()
	{
		//CG_TRACE("Deleted GLVertexArray {0}", m_id);
		glDeleteVertexArrays(1, &m_id);
	}

	void GLVertexArray::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void GLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}
