#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

namespace CGEngine::Math
{
	constexpr Vector3 X_AXIS = { 1.0f, 0.0f, 0.0f };
	constexpr Vector3 Y_AXIS = { 0.0f, 1.0f, 0.0f };
	constexpr Vector3 Z_AXIS = { 0.0f, 0.0f, 1.0f };

	// out: degrees
	float RadToDeg(float radians);
	// out: radians
	float DegToRad(float degrees);

	// out: degrees
	Vector3 RadToDeg(const Vector3& radians);
	// out: radians
	Vector3 DegToRad(const Vector3& degrees);

	const void* value_ptr(const Vector2& vector);
	const void* value_ptr(const Vector3& vector);
	const void* value_ptr(const Vector4& vector);
	const void* value_ptr(const Mat3& matrix);
	const void* value_ptr(const Mat4& matrix);

	Vector2 make_vec2(const float* data);
	Vector3 make_vec3(const float* data);
	Vector4 make_vec4(const float* data);
	Mat3	make_mat3(const float* data);
	Mat4	make_mat4(const float* data);
}