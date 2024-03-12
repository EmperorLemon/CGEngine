#include "GUID.h"

namespace CGEngine::Utils
{
	GUID::GUID()
	{
		m_guid = xg::newGuid();
	}

	void GUID::reset()
	{
		m_guid = xg::newGuid();
	}

	bool GUID::valid() const
	{
		return m_guid.isValid();
	}

	std::string GUID::str() const
	{
		return m_guid.str();
	}

	bool GUID::operator==(const GUID& other) const
	{
		return m_guid == other.m_guid;
	}

	bool GUID::operator!=(const GUID& other) const
	{
		return m_guid != other.m_guid;
	}

	bool GUID::operator<(const GUID& other) const
	{
		return m_guid < other.m_guid;
	}


}