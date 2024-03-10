#pragma once

#include "Types.h"

#include <cstdint>
#include <optional>

namespace CGEngine
{
	struct BufferInfo
	{
		size_t  size   = 0; // byte size of buffer
		size_t  count  = 0; // number of elements in buffer
		size_t  offset = 0; // byte offset of buffer (used for combining multiple buffers)
	};

	class Buffer
	{
	public:
		explicit Buffer(const BufferTarget target) : p_target(target) {}

		Buffer(Buffer&&) noexcept = default;
		Buffer& operator=(Buffer&&) noexcept = delete;
		Buffer(const Buffer&) noexcept = delete;
		Buffer& operator=(const Buffer&) noexcept = delete;

		virtual ~Buffer() = default;

		virtual void SetData(size_t size, const void* data) const = 0;
		virtual void SetSubData(size_t offset, size_t size, const void* data) const = 0;

		virtual void BindBufferBase(uint32_t binding)  const = 0;
		virtual void BindBufferRange(uint32_t binding, size_t offset, size_t size) const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;
	protected:
		uint32_t p_id = 0;
		BufferTarget p_target = BufferTarget::NONE;
	};

	class VertexArray
	{
	public:
		explicit VertexArray(const BufferInfo& vertexBuffer) : p_vertexBuffer(vertexBuffer) {}

		VertexArray(VertexArray&&) noexcept = default;
		VertexArray& operator=(VertexArray&&) noexcept = delete;
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		virtual ~VertexArray() = default;

		virtual void Bind()   const = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;

		[[nodiscard]] virtual const BufferInfo& GetVertices() const = 0;
		[[nodiscard]] virtual const BufferInfo& GetIndices()  const = 0;
	protected:
		uint32_t p_id = 0;

		const BufferInfo&		  p_vertexBuffer;
		std::optional<BufferInfo> p_indexBuffer;
	};
}
