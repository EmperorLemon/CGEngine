#define GLM_ENABLE_EXPERIMENTAL
#include "Math.h"

#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/matrix_decompose.hpp>

namespace CGEngine::Math
{
	float RadToDeg(const float radians)
	{
		return glm::degrees(radians);
	}

	float DegToRad(const float degrees)
	{
		return glm::radians(degrees);
	}

	Vec3 RadToDeg(const Vec3& radians)
	{
		return degrees(radians);
	}

	Vec3 DegToRad(const Vec3& degrees)
	{
		return radians(degrees);
	}

	float Sin(const float angle)
	{
		return glm::sin(angle);
	}

	float Cos(const float angle)
	{
		return glm::cos(angle);
	}

	float Tan(const float angle)
	{
		return glm::tan(angle);
	}

	const float* ToArray(const Vec2& vector)
	{
		return value_ptr(vector);
	}

	const float* ToArray(const Vec3& vector)
	{
		return value_ptr(vector);
	}

	const float* ToArray(const Vec4& vector)
	{
		return value_ptr(vector);
	}

	const float* ToArray(const Mat3& matrix)
	{
		return value_ptr(matrix);
	}

	const float* ToArray(const Mat4& matrix)
	{
		return value_ptr(matrix);
	}

	float* ToPtr(Mat4& matrix)
	{
		return value_ptr(matrix);
	}

	Vec2 ToVec2(const float* data)
	{
		return glm::make_vec2(data);
	}

	Vec3 ToVec3(const float* data)
	{
		return glm::make_vec3(data);
	}

	Vec4 ToVec4(const float* data)
	{
		return glm::make_vec4(data);
	}

	Quat ToQuat(const float* data)
	{
		return glm::make_quat(data);
	}

	Quat ToQuat(const Mat3& data)
	{
		return quat_cast(data);
	}

	Quat ToQuat(const Mat4& data)
	{
		return quat_cast(data);
	}

	Mat3 ToMat3(const float* data)
	{
		return glm::make_mat3x3(data);
	}

	Mat4 ToMat4(const float* data)
	{
		return glm::make_mat4x4(data);
	}

	Mat4 ToMat4(const Quat& data)
	{
		return mat4_cast(data);
	}

	void Decompose(const Mat4& matrix, Vec3& position, Quat& rotation, Vec3& scale)
	{
		auto skew = Vec3(0.0f);
		auto perspective = Vec4(0.0f);

		decompose(matrix, scale, rotation, position, skew, perspective);
	}

}