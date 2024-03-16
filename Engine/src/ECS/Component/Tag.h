#pragma once

#include <string>

#include "Utils/GUID.h"

namespace CGEngine::Component
{
	struct Tag
	{
		std::string name;
		Utils::GUID guid;
	};
}