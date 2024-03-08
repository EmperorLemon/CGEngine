#include "Camera.h"

#include "Math/Math.h"

namespace CGEngine
{
	float GetAspectRatio(const int32_t width, const int32_t height)
	{
		if (height != 0) return static_cast<float>(width) / static_cast<float>(height);

		return static_cast<float>(width) / 1.0f;
	}

	float GetFieldOfView(const float degrees)
	{
		return Math::DegToRad(degrees);
	}
}