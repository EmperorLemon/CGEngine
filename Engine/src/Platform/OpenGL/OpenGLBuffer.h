#pragma once

#include "Renderer/Buffer.h"

namespace CGEngine::OpenGL
{
	class GLBuffer final : public Buffer
	{
	public:
		GLBuffer(size_t size, const void* data);

		GLBuffer(GLBuffer&&) noexcept = default;
		GLBuffer& operator=(GLBuffer&&) noexcept = default;
		GLBuffer(const GLBuffer&) = delete;
		GLBuffer& operator=(const GLBuffer&) = delete;

		~GLBuffer() override;

		void SetData(size_t size, const void* data) const override;
		void SetSubData(size_t offset, size_t size, const void* data) const override;

		[[nodiscard]] size_t GetSize() const override { return p_size; }
		[[nodiscard]] uint32_t GetID() const override { return p_id; }
	};

	class GLVertexArray final : public VertexArray
	{
	public:
		GLVertexArray(const GLBuffer* vertexBuffer, const VertexLayout& layout);

		GLVertexArray(GLVertexArray&&) noexcept = default;
		GLVertexArray& operator=(GLVertexArray&&) noexcept = default;
		GLVertexArray(const GLVertexArray&) = delete;
		GLVertexArray& operator=(const GLVertexArray&) = delete;

		~GLVertexArray() override;

		void Bind()   const override;
		void Unbind() const override;

		[[nodiscard]] uint32_t GetID() const override		   { return p_id; }

		[[nodiscard]] int32_t GetVertexCount() const override  { return p_vertexCount; }
		[[nodiscard]] int32_t GetIndexCount()  const override  { return p_indexCount;  }
	};
}