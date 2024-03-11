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
		GLBuffer(BufferTarget target, size_t size, const void* data);

		GLBuffer(GLBuffer&&) noexcept = default;
		GLBuffer(const GLBuffer&) noexcept = delete;
		GLBuffer& operator=(GLBuffer&&) noexcept = delete;
		GLBuffer& operator=(const GLBuffer&) noexcept = delete;

		~GLBuffer() override;

		void SetData(size_t size, const void* data) const override;
		void SetSubData(size_t offset, size_t size, const void* data) const override;

		void BindBufferBase(uint32_t binding) const override;
		void BindBufferRange(uint32_t binding, size_t offset, size_t size) const override;

		[[nodiscard]] uint32_t GetID() const override { return p_id; }
	};

	class GLVertexArray final : public VertexArray
	{
	public:
		GLVertexArray(uint32_t bufferID, const BufferInfo& vertexBuffer, const BufferInfo* indexBuffer, const VertexLayout& layout);

		GLVertexArray(GLVertexArray&&) noexcept = default;
		GLVertexArray(const GLVertexArray&) noexcept = delete;
		GLVertexArray& operator=(GLVertexArray&&) noexcept = delete;
		GLVertexArray& operator=(const GLVertexArray&) noexcept = delete;

		~GLVertexArray() override;

		void Bind()   const override;
		void Unbind() const override;

		[[nodiscard]] uint32_t GetID() const override { return p_id; }

		void SetDrawType(const DrawType type) { m_drawType = type; }
		[[nodiscard]] DrawType GetDrawType() const { return m_drawType; }

		[[nodiscard]] const BufferInfo& GetVertices() const override { return p_vertexBuffer; }
		[[nodiscard]] const BufferInfo& GetIndices()  const override { return p_indexBuffer; }

	private:
		DrawType m_drawType = DrawType::DRAW_ARRAYS;
	};
}