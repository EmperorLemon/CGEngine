#pragma once

#include "Math/Vector3.h"
#include "Math/Quaternion.h"

#include "Math/Matrix4x4.h"

namespace CGEngine::Math
{
	Mat4 Orthographic(float left, float right, float bottom, float top, float near = 0.1f, float far = 1000.0f);
	Mat4 Perspective(float fov, float aspect, float near = 0.1f, float far = 1000.0f);
	Mat4	    View(const Vec3& position, const Vec3& direction, const Vec3& up);

	Mat4 Translate(const Mat4& transform, const Vec3& translation);
	Mat4    Rotate(const Mat4& transform, float angle, const Vec3& axis);
	Mat4	 Scale(const Mat4& transform, const Vec3& scale);
}