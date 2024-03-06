#pragma once

#include <vector>
#include <cstdint>
#include <optional>

namespace CGEngine
{
	enum class DataType : uint8_t
	{
		VOID,
		UNSIGNED_BYTE,
		BYTE,
		UNSIGNED_SHORT,
		SHORT,
		UNSIGNED_INT,
		INT,
		FLOAT,
	};

	struct VertexAttribute
	{
		uint32_t index;
		int32_t  count;
		DataType type;
		bool	 normalized;
		uint32_t offset;
		int32_t  stride;
	};

	constexpr int32_t GetAttributeTypeSize(const DataType type)
	{
		switch (type)
		{
		case DataType::VOID:  return 0 << 0;
		case DataType::UNSIGNED_BYTE:
		case DataType::BYTE:  return 1 << 0;
		case DataType::UNSIGNED_SHORT:
		case DataType::SHORT: return 1 << 1;
		case DataType::UNSIGNED_INT:
		case DataType::INT:
		case DataType::FLOAT: return 1 << 2;
		}

		return 0;
	}

	class VertexLayout
	{
	public:
		VertexLayout& add(uint32_t index, int32_t count, DataType type, uint32_t offset, int32_t stride, bool normalized = false);
		VertexLayout& end();

		[[nodiscard]] const std::vector<VertexAttribute>& GetAttributes() const { return m_layout; }
	private:
		uint32_t m_offset = 0;
		std::vector<VertexAttribute> m_layout;
	};

	struct BufferInfo
	{
		size_t  size   = 0; // byte size of buffer
		size_t  count  = 0; // number of elements in buffer
		size_t  offset = 0; // byte offset of buffer (used for combining multiple buffers)
	};

	class Buffer
	{
	public:
		Buffer() = default;

		Buffer(Buffer&&) noexcept = default;
		Buffer& operator=(Buffer&&) noexcept = delete;
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		virtual ~Buffer() = default;

		virtual void SetData(size_t size, const void* data) const = 0;
		virtual void SetSubData(size_t offset, size_t size, const void* data) const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;
	protected:
		uint32_t p_id = 0;
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
