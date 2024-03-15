#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Quaternion.h"

#include "Matrix3x3.h"
#include "Matrix4x4.h"

namespace CGEngine::Math
{
	constexpr Vec3 X_AXIS = { 1.0f, 0.0f, 0.0f };
	constexpr Vec3 Y_AXIS = { 0.0f, 1.0f, 0.0f };
	constexpr Vec3 Z_AXIS = { 0.0f, 0.0f, 1.0f };

	// out: degrees
	float RadToDeg(float radians);
	// out: radians
	float DegToRad(float degrees);

	float Sin(float angle);
	float Cos(float angle);
	float Tan(float angle);

	// out: degrees
	Vec3 RadToDeg(const Vec3& radians);
	// out: radians
	Vec3 DegToRad(const Vec3& degrees);

	const float* ToArray(const Vec2& vector);
	const float* ToArray(const Vec3& vector);
	const float* ToArray(const Vec4& vector);
	const float* ToArray(const Mat3& matrix);
	const float* ToArray(const Mat4& matrix);
	float* ToPtr(Vec3& vector);
	float* ToPtr(Mat4& matrix);

	Vec2 ToVec2(const float* data);
	Vec3 ToVec3(const float* data);
	Vec4 ToVec4(const float* data);
	Quat ToQuat(const float* data);
	Quat ToQuat(const Mat3& data);
	Quat ToQuat(const Mat4& data);
	Mat3 ToMat3(const float* data);
	Mat4 ToMat4(const float* data);
	Mat4 ToMat4(const Quat&  data);

	void Decompose(const Mat4& matrix, Vec3& position, Quat& rotation, Vec3& scale);

	Mat4 ViewLerp(const Mat4& viewA, const Mat4& viewB, float t);
}