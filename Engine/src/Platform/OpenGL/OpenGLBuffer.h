#pragma once

#include <string_view>

#include "Renderer/Buffer.h"

namespace CGEngine::OpenGL
{
	class GLBuffer final : public Buffer
	{
	public:
		GLBuffer(const void* data, size_t size, int32_t count, std::string_view name);

		[[nodiscard]] size_t   GetSize()  const override { return p_size; }
		[[nodiscard]] int32_t  GetCount() const override { return p_count; }
		[[nodiscard]] uint32_t GetID()    const override { return p_id; }
	};

	class GLVertexArray final : public VertexArray
	{
	public:
		GLVertexArray(const GLBuffer* vertexBuffer, const GLBuffer* indexBuffer, const VertexLayout& layout);

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