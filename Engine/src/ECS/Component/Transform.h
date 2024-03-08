#pragma once

#include "Math/Vector3.h"
#include "Math/Quaternion.h"

namespace CGEngine::Component
{
	struct Transform
	{
		Math::Vector3 position;
		Math::Quat    rotation;
		Math::Vector3 scale;
	};
}