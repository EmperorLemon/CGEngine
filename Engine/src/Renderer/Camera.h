#pragma once

#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"

namespace CGEngine
{
	struct Camera
	{
		Math::Vec3  position = Math::Vec3(7.0f, 1.0f,  7.0f);
		Math::Vec3 direction = Math::Vec3(0.0f, 0.0f,  -1.0f);
		Math::Vec3		 up = Math::Vec3(0.0f, 1.0f,   0.0f);

		float fov	 = 0.0f;
		float aspect = 1.0f;
		float near   = 0.1f;
		float far    = 1000.0f;

		Math::Mat4 view = Math::Mat4(1.0f);
		Math::Mat4 projection = Math::Mat4(1.0f);
	};

	float GetFieldOfView(float degrees);
	float GetAspectRatio(int32_t width, int32_t height);
}