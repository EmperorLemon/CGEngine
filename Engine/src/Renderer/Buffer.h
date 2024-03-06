#pragma once

#include <vector>
#include <cstdint>
#include <span>

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
		VertexLayout& add(uint32_t index, int32_t count, uint32_t offset, int32_t stride, bool normalized = false);
		VertexLayout& end();

		[[nodiscard]] const std::vector<VertexAttribute>& GetAttributes() const { return m_layout; }
	private:
		uint32_t m_offset = 0;
		std::vector<VertexAttribute> m_layout;
	};

	struct SubBuffer
	{
		explicit SubBuffer(const size_t length, const size_t stride, const size_t offset = 0, const void* data = nullptr) : length(length), stride(stride), offset(static_cast<int32_t>(offset)), data(data) {}

		size_t  length = 0;
		size_t  stride = 0;
		int32_t offset = 0;
		const void* data = nullptr;
	};

	class Buffer
	{
	public:
		explicit Buffer(const size_t size) : p_size(size) {}

		Buffer(Buffer&&) noexcept = default;
		Buffer& operator=(Buffer&&) noexcept = default;
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		virtual ~Buffer() = default;

		virtual void SetData(size_t size, const void* data) const = 0;
		virtual void SetSubData(size_t offset, size_t size, const void* data) const = 0;

		[[nodiscard]] virtual size_t GetSize() const = 0;
		[[nodiscard]] virtual uint32_t GetID() const = 0;
	protected:
		size_t	 p_size = 0;
		uint32_t p_id   = 0;
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
