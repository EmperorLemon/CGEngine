#pragma once

#include "Math/Vector3.h"

namespace CGEngine::Component
{
	struct Transform
	{
		Math::Vector3 position = Math::Vector3(0.0f);
		Math::Vector3 rotation = Math::Vector3(0.0f);
		Math::Vector3 scale    = Math::Vector3(1.0f);
	};
}