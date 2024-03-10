#pragma once

#include <cstdint>

namespace CGEngine::Assets
{
	enum class LightType : uint8_t
	{
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		AREA_LIGHT
	};

	struct Light
	{
		LightType type  = LightType::DIRECTIONAL_LIGHT;

		float linear	= 0.045f;
		float quadratic = 0.0075f;

		float cutOff      =  7.5f;
		float outerCutOff = 12.5f;
	};
}