#pragma once

#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"

namespace CGEngine
{
	struct Camera
	{
		Math::Vector3  position = Math::Vector3(0.0f, 0.0f,  10.0f);
		Math::Vector3 direction = Math::Vector3(0.0f, 0.0f,  -1.0f);
		Math::Vector3		 up = Math::Vector3(0.0f, 1.0f,   0.0f);

		float fov	 = 0.0f;
		float near   = 0.1f;
		float far    = 1000.0f;
	};

	float GetFieldOfView(float degrees);
	float GetAspectRatio(int32_t width, int32_t height);
}