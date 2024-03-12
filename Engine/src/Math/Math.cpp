#include "Math.h"

#include <glm/trigonometric.hpp>

#include <glm/gtc/type_ptr.hpp>

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

	Vector3 RadToDeg(const Vector3& radians)
	{
		return degrees(radians);
	}

	Vector3 DegToRad(const Vector3& degrees)
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

	const float* value_ptr(const Vector2& vector)
	{
		return glm::value_ptr(vector);
	}

	const float* value_ptr(const Vector3& vector)
	{
		return glm::value_ptr(vector);
	}

	const float* value_ptr(const Vector4& vector)
	{
		return glm::value_ptr(vector);
	}

	const float* value_ptr(const Mat3& matrix)
	{
		return glm::value_ptr(matrix);
	}

	const float* value_ptr(const Mat4& matrix)
	{
		return glm::value_ptr(matrix);
	}

	Vector2 make_vec2(const float* data)
	{
		return glm::make_vec2(data);
	}

	Vector3 make_vec3(const float* data)
	{
		return glm::make_vec3(data);
	}

	Vector4 make_vec4(const float* data)
	{
		return glm::make_vec4(data);
	}

	Mat3 make_mat3(const float* data)
	{
		return glm::make_mat3x3(data);
	}

	Mat4 make_mat4(const float* data)
	{
		return glm::make_mat4x4(data);
	}
}