#include "Buffer.h"

namespace CGEngine
{
	VertexLayout& VertexLayout::add(const uint32_t index, const int32_t count, const DataType type, const bool normalized)
	{
		m_layout.emplace_back(index, count, type, normalized, 0);

		return *this;
	}

	VertexLayout& VertexLayout::end()
	{
		m_offset = 0;

		for (auto& attribute : m_layout)
		{
			const uint32_t size = attribute.count * static_cast<int32_t>(attribute.type);
			attribute.offset = m_offset;
			m_offset += size;
		}

		return *this;
	}
}