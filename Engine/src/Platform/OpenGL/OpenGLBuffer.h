#pragma once

#include "Renderer/Buffer.h"

namespace CGEngine
{
	class VertexLayout;
}
namespace CGEngine::OpenGL
{
	class GLBuffer final : public Buffer
	{
	public:
		GLBuffer(size_t size, const void* data);

		GLBuffer(GLBuffer&&) noexcept = delete;
		GLBuffer(const GLBuffer&) noexcept = delete;
		GLBuffer& operator=(GLBuffer&&) noexcept = delete;
		GLBuffer& operator=(const GLBuffer&) noexcept = delete;

		~GLBuffer() override;

		void SetData(size_t size, const void* data) const override;
		void SetSubData(size_t offset, size_t size, const void* data) const override;

		[[nodiscard]] uint32_t GetID() const override { return p_id; }
	};

	class GLVertexArray final : public VertexArray
	{
	public:
		GLVertexArray(uint32_t bufferID, const BufferInfo& vertexBuffer, const BufferInfo* indexBuffer, const VertexLayout& layout);

		GLVertexArray(GLVertexArray&&) noexcept = default;
		GLVertexArray& operator=(GLVertexArray&&) noexcept = delete;
		GLVertexArray(const GLVertexArray&) = delete;
		GLVertexArray& operator=(const GLVertexArray&) = delete;

		~GLVertexArray() override;

		void Bind()   const override;
		void Unbind() const override;

		[[nodiscard]] uint32_t GetID() const override { return p_id; }

		[[nodiscard]] const BufferInfo& GetVertices() const override { return p_vertexBuffer; }
		[[nodiscard]] const BufferInfo&  GetIndices() const override { if (p_indexBuffer.has_value()) return p_indexBuffer.value(); return p_vertexBuffer; }
	};
}