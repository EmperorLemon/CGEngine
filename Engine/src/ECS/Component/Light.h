#pragma once

#include "Math/Vector4.h"

namespace CGEngine::Component
{
	struct Light
	{
		Math::Vec4 direction = Math::Vec4(0.0f);

		float linear = 0.045f;
		float quadratic = 0.0075f;

		float cutOff = 12.5f;
		float outerCutOff = 17.5f;
	};
}