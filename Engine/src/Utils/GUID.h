#pragma once

#include <crossguid/guid.hpp>

namespace CGEngine::Utils
{
	class GUID
	{
	public:
		GUID();

		void reset();

		[[nodiscard]] bool valid() const;

		bool operator==(const GUID& other) const;
		bool operator!=(const GUID& other) const;
		bool operator<(const GUID& other) const;
	private:
		xg::Guid m_guid;
	};
}