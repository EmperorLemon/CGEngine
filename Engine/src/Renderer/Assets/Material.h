#pragma once

#include "Math/Vector4.h"

namespace CGEngine::Assets
{
	struct Material
	{
		Math::Vector4 albedo  = Math::Vector4(1.0f);
		float  metallicFactor = 1.0f;
		float roughnessFactor = 1.0f;
	};
}