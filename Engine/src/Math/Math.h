#pragma once

#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"

namespace CGEngine::Math
{
	// out: degrees
	float RadToDeg(float radians);
	// out: radians
	float DegToRad(float degrees);

	// out: degrees
	Vector3 RadToDeg(const Vector3& radians);
	// out: radians
	Vector3 DegToRad(const Vector3& degrees);

	const void* value_ptr(const Mat4& matrix);
}