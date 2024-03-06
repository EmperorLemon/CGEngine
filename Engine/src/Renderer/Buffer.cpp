#include "Buffer.h"

namespace CGEngine
{
	VertexLayout& VertexLayout::add(const uint32_t index, const int32_t count, const DataType type, const uint32_t offset, const int32_t stride, bool normalized)
	{
		m_layout.emplace_back(index, count, type, normalized, offset, stride);

		return *this;
	}

	VertexLayout& VertexLayout::end()
	{
		m_offset = 0;

		for (auto& attribute : m_layout)
		{
			const int32_t stride = attribute.stride;
			attribute.offset = m_offset;
			m_offset += stride;
		}

		return *this;
	}
}
