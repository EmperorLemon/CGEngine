#include "Buffer.h"

namespace CGEngine
{
	VertexLayout& VertexLayout::add(const uint32_t index, const int32_t count, const DataType type, const bool normalized)
	{
		m_ignore = false;
		m_layout.emplace_back(index, count, type, normalized, 0, count * GetDataSize(type));

		return *this;
	}

	VertexLayout& VertexLayout::add(const uint32_t index, const int32_t count, const uint32_t offset, const int32_t stride, bool normalized)
	{
		m_ignore = true;
		m_layout.emplace_back(index, count, DataType::VOID, normalized, offset, stride);

		return *this;
	}

	VertexLayout& VertexLayout::end()
	{
		if (!m_ignore)
		{
			m_offset = 0;

			for (auto& attribute : m_layout)
			{
				const int32_t size = attribute.stride;
				attribute.offset = m_offset;
				m_offset += size;
			}
		}


		return *this;
	}
}
