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
		explicit Buffer(const uint32_t id) : p_id(id) {}

		Buffer(Buffer&&) noexcept = default;
		Buffer(const Buffer&) noexcept = delete;
		Buffer& operator=(Buffer&&) noexcept = default;
		Buffer& operator=(const Buffer&) noexcept = default;

		virtual ~Buffer() = default;

		virtual void SetData(size_t size, const void* data) const = 0;
		virtual void SetSubData(size_t offset, size_t size, const void* data) const = 0;

		virtual void BindBufferBase(uint32_t binding)  const = 0;
		virtual void BindBufferRange(uint32_t binding, size_t offset, size_t size) const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;
	protected:
		uint32_t p_id = 0;
	};

	class VertexArray
	{
	public:
		explicit VertexArray(const uint32_t id) : p_id(id) {}

		VertexArray(VertexArray&&) noexcept = default;
		VertexArray(const VertexArray&) noexcept = delete;
		VertexArray& operator=(VertexArray&&) noexcept = delete;
		VertexArray& operator=(const VertexArray&) noexcept = delete;

		virtual ~VertexArray() = default;

		virtual void Bind()   const = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;

		[[nodiscard]] virtual const BufferInfo& GetVertices() const = 0;
		[[nodiscard]] virtual const BufferInfo& GetIndices()  const = 0;

		virtual void SetDrawType(DrawType type) = 0;
	protected:
		uint32_t p_id = 0;
	};
}
