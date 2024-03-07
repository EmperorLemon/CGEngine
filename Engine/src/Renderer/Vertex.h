#pragma once

#include <vector>
#include <array>

namespace CGEngine
{
#define BIT(x) (1 << (x))

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
	};

	constexpr int32_t GetAttributeTypeSize(const DataType type)
	{
		switch (type)
		{
		case DataType::VOID:
		case DataType::UNSIGNED_BYTE:
		case DataType::BYTE:  return BIT(0);
		case DataType::UNSIGNED_SHORT:
		case DataType::SHORT: return BIT(1);
		case DataType::UNSIGNED_INT:
		case DataType::INT:
		case DataType::FLOAT: return BIT(2);
		}

		return 0;
	}

	struct Vertex
	{
		std::array<float, 3> position;
		std::array<float, 3> normal;
		std::array<float, 2> uv;
	};

	class VertexLayout
	{
	public:
		VertexLayout& add(const uint32_t index, const int32_t count, const DataType type, const uint32_t offset, const bool normalized = false)
		{
			m_layout.emplace_back(index, count, type, normalized, offset);

			return *this;
		}

		VertexLayout& add(const uint32_t index, const int32_t count, const DataType type, bool normalized = false)
		{
			m_layout.emplace_back(index, count, type, normalized, 0);

			return *this;
		}

		VertexLayout& end()
		{
			int32_t offset = 0;
			m_stride = 0;

			for (auto& attribute : m_layout)
			{
				const int32_t size = attribute.count * GetAttributeTypeSize(attribute.type);
				attribute.offset = offset;
				offset += size;
			}

			m_stride = offset;

			return *this;
		}

		[[nodiscard]] int32_t GetStride() const { return m_stride; }
		[[nodiscard]] const std::vector<VertexAttribute>& GetAttributes() const { return m_layout; }
	private:
		int32_t m_stride = 0;
		std::vector<VertexAttribute> m_layout;
	};
}