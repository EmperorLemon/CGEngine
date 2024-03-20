#pragma once

#include "Math/Vector2.h"
#include "Math/Vector4.h"

namespace CGEngine::Assets
{
	struct Material
	{
		Math::Vec4 albedo  = Math::Vec4(1.0f);
		float  metallicFactor = 1.0f;
		float roughnessFactor = 1.0f;
	};
}