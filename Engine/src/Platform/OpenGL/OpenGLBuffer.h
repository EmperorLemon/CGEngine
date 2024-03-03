#pragma once

#include "Renderer/Buffer.h"

namespace CGEngine::OpenGL
{
	class GLBuffer final : public Buffer
	{
	public:
		GLBuffer(const void* data, size_t size, std::string_view name);

		[[nodiscard]] uint32_t GetID()   const override { return p_id; }
		[[nodiscard]] size_t   GetSize() const override { return p_size; }
	};

	class GLVertexArray
	{
	public:
		GLVertexArray(const GLBuffer* vertexBuffer, const GLBuffer* indexBuffer, size_t count, const VertexLayout& layout);

		GLVertexArray(GLVertexArray&& old) noexcept = default;
		GLVertexArray& operator=(GLVertexArray&& old) noexcept = default;
		GLVertexArray(const GLVertexArray&) = delete;
		GLVertexArray& operator=(const GLVertexArray&) = delete;

		~GLVertexArray();

		void Bind()   const;
		void Unbind() const;
	private:
		uint32_t m_id = 0;
	};
}