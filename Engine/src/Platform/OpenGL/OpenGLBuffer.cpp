#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace CGEngine::OpenGL
{
	GLenum Convert(const DataType type)
	{
		if (type == DataType::INT8) return GL_BYTE;
		if (type == DataType::UINT8) return GL_UNSIGNED_BYTE;

		if (type == DataType::INT16) return GL_SHORT;
		if (type == DataType::UINT16) return GL_UNSIGNED_SHORT;

		if (type == DataType::INT32) return GL_INT;
		if (type == DataType::UINT32) return GL_UNSIGNED_INT;

		if (type == DataType::FLOAT) return GL_FLOAT;

		return GL_FLOAT;
	}

	GLBuffer::GLBuffer(const void* data, const size_t size, const std::string_view& name) : Buffer(size, 0)
	{
		glCreateBuffers(1, &p_id);
		glNamedBufferStorage(p_id, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_STORAGE_BIT);

		if (!name.empty())
			glObjectLabel(GL_BUFFER, p_id, static_cast<GLsizei>(name.length()), name.data());
	}

	GLVertexArray::GLVertexArray(const GLBuffer* vertexBuffer, const GLBuffer* indexBuffer, const size_t count, const VertexLayout& layout)
	{
		glCreateVertexArrays(1, &m_id);

		glVertexArrayVertexBuffer(m_id, 0, vertexBuffer->GetID(), 0, static_cast<GLsizei>(vertexBuffer->GetSize() / count));
		if (indexBuffer != nullptr)  glVertexArrayElementBuffer(m_id, indexBuffer->GetID());

		const std::vector<VertexAttribute>& attributes = layout.GetAttributes();

		for (const auto& attribute : attributes)
			glEnableVertexArrayAttrib(m_id, attribute.index);

		for (const auto& attribute : attributes)
			glVertexArrayAttribFormat(m_id, attribute.index, attribute.count, Convert(attribute.type), attribute.normalized, attribute.offset);

		for (const auto& attribute : attributes)
			glVertexArrayAttribBinding(m_id, attribute.index, 0);
	}

	GLVertexArray::~GLVertexArray()
	{
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