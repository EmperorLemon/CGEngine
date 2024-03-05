#pragma once

#include <vector>
#include <cstdint>

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
		int32_t count;
		DataType type;
		bool normalized;
		uint32_t offset;
		int32_t stride;
	};

	constexpr int32_t GetDataSize(const DataType type)
	{
		switch (type)
		{
		case DataType::VOID:
			return 0 << 0;
		case DataType::UNSIGNED_BYTE:
		case DataType::BYTE:
			return 1 << 0;
		case DataType::UNSIGNED_SHORT:
		case DataType::SHORT:
			return 1 << 1;
		case DataType::UNSIGNED_INT:
		case DataType::INT:
		case DataType::FLOAT:
			return 1 << 2;
		}

		return 1 << 2;
	}

	class VertexLayout
	{
	public:
		VertexLayout& add(uint32_t index, int32_t count, DataType type, bool normalized = false);
		VertexLayout& add(uint32_t index, int32_t count, int32_t stride, bool normalized = false);
		VertexLayout& end();

		[[nodiscard]] const std::vector<VertexAttribute>& GetAttributes() const { return m_layout; }
	private:
		uint32_t m_offset = 0;
		std::vector<VertexAttribute> m_layout;
	};

	struct SubBuffer
	{
		SubBuffer(const DataType type, const size_t length, const int32_t offset = 0) : type(type), size(GetDataSize(type)), length(static_cast<int32_t>(length)), offset(offset) {}

		DataType type = DataType::VOID;
		int32_t  size = 0;
		int32_t  length = 0;
		int32_t  offset = 0;
	};

	class Buffer
	{
	public:
		Buffer(const DataType type, const int32_t size, const int32_t length, const uint32_t id) : p_type(type), p_size(size), p_length(length), p_id(id) {}

		Buffer(Buffer&&) noexcept = default;
		Buffer& operator=(Buffer&&) noexcept = default;
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		virtual ~Buffer() = default;

		[[nodiscard]] virtual int32_t  GetSize()    const = 0;
		[[nodiscard]] virtual int32_t  GetLength()  const = 0;
		[[nodiscard]] virtual uint32_t GetID()      const = 0;
	protected:
		DataType p_type  = DataType::VOID;
		int32_t	 p_size  = 0;
		int32_t  p_length = 0;
		uint32_t p_id    = 0;
	};

	class VertexArray
	{
	public:
		explicit VertexArray(const uint32_t id, const int32_t vertexCount, const int32_t indexCount) : p_id(id), p_vertexCount(vertexCount), p_indexCount(indexCount) {}

		VertexArray(VertexArray&&) noexcept = default;
		VertexArray& operator=(VertexArray&&) noexcept = default;
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		virtual ~VertexArray() = default;

		virtual void Bind()   const = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] virtual uint32_t GetID()   const = 0;

		[[nodiscard]] virtual int32_t  GetVertexCount() const = 0;
		[[nodiscard]] virtual int32_t  GetIndexCount()  const = 0;
	protected:
		uint32_t p_id = 0;

		int32_t  p_vertexCount = 0;
		int32_t  p_indexCount = 0;
	};
}
