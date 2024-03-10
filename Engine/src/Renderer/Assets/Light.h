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
		Math::Vector4 position = Math::Vector4(Math::Vector3(1.0f, 0.0f, 0.0f), 0.0f);
		//Math::Vector3 direction = Math::Vector3(0.0f);

		//LightType type = LightType::DIRECTIONAL_LIGHT;

		//float linear	= 0.045f;
		//float quadratic = 0.0075f;

		//float cutOff      =  7.5f;
		//float outerCutOff = 12.5f;
	};
}