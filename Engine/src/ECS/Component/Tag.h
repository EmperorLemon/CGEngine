#pragma once

#include <string>

#include "Utils/GUID.h"

namespace CGEngine::Component
{
	enum class EntityType : uint8_t
	{
		NONE,
		DRAWOBJECT,
		INSTANCE,
		LIGHT
	};

	struct Tag
	{
		std::string   name;
		EntityType    type = EntityType::NONE;
		Utils::GUID   guid;
	};
}