#pragma once

#include <cstdint>

#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace CGEngine::Assets
{
	enum class LightType : uint32_t
	{
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
	};

	struct Light
	{
		// set w component to 0 for directional and 1 for using light position
		Math::Vector4 direction = Math::Vector4(0.0f); 

		LightType type			= LightType::DIRECTIONAL_LIGHT;

		float linear			= 0.045f;
		float quadratic			= 0.0075f;

		float cutOff			= 12.5f;
		float outerCutOff		= 17.5f;
	};
}