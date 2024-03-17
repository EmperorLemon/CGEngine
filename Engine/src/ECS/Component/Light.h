#pragma once

#include "Math/Vector2.h"
#include "Math/Vector4.h"

namespace CGEngine::Component
{
	enum class LightType : uint32_t
	{
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
	};

	struct Light
	{
		Math::Vec4 direction	 = Math::Vec4(0.0f);

		Math::Vec4 diffuseColor  = Math::Vec4(1.0f);
		Math::Vec4 specularColor = Math::Vec4(1.0f);

		float constant = 1.0f;
		float linear = 0.045f;
		float quadratic = 0.0075f;

		float innerSpotAngle = 0.0f;
		float outerSpotAngle = 1.0f;

		LightType  type    = LightType::POINT_LIGHT;
		Math::Vec2 padding = Math::Vec2(0.0f);
	};
}