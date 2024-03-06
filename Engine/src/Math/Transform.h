#pragma once

#include "Math/Vector3.h"
#include "Math/Quaternion.h"

#include "Math/Matrix4x4.h"

namespace CGEngine::Math
{
	Mat4 Perspective(float fov, float aspect, float near = 0.1f, float far = 1000.0f);
	Mat4	    View(const Vector3& position, const Vector3& direction, const Vector3& up);

	Mat4 Translate(const Mat4& transform, const Vector3& translation);
	Mat4    Rotate(const Mat4& transform, float angle, const Vector3& axis);
	Mat4	 Scale(const Mat4& transform, const Vector3& scale);
}