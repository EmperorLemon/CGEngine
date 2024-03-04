#pragma once

#include <string_view>
#include <vector>
#include <cstdint>

namespace CGEngine
{
	enum class DataType : size_t
	{
		UINT8  = 1 << 0,
		INT8   = 1 << 0,
		INT16  = 1 << 1,
		UINT16 = 1 << 1,
		UINT32 = 1 << 2,
		INT32  = 1 << 2,
		FLOAT  = 1 << 2
	};

	struct VertexAttribute
	{
		uint32_t index;
		 int32_t count;
		DataType type;
			bool normalized;
		uint32_t offset;
	};

	struct Vertex
	{
		float position[3];
		float uv[2];
		float normal[3];
	};

	class VertexLayout
	{
	public:
		VertexLayout& add(uint32_t index, int32_t count, DataType type, bool normalized = false);
		VertexLayout& end();

		[[nodiscard]] const std::vector<VertexAttribute>& GetAttributes() const { return m_layout; }
	private:
		uint32_t m_offset = 0;
		std::vector<VertexAttribute> m_layout;
	};

	class Buffer
	{
	public:
		Buffer(const size_t size, const uint32_t id) : p_size(size), p_id(id) {}

		Buffer(Buffer&&) noexcept = default;
		Buffer& operator=(Buffer&&) noexcept = default;
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		virtual ~Buffer() = default;

		[[nodiscard]] virtual uint32_t GetID()   const = 0;
		[[nodiscard]] virtual size_t   GetSize() const = 0;
	protected:
		size_t	 p_size = 0;
		uint32_t p_id = 0;
	};
}
