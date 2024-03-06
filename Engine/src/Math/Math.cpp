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

	const void* value_ptr(const Vector2& vector)
	{
		return glm::value_ptr(vector);
	}

	const void* value_ptr(const Vector3& vector)
	{
		return glm::value_ptr(vector);
	}

	const void* value_ptr(const Vector4& vector)
	{
		return glm::value_ptr(vector);
	}

	const void* value_ptr(const Mat3& matrix)
	{
		return glm::value_ptr(matrix);
	}

	const void* value_ptr(const Mat4& matrix)
	{
		return glm::value_ptr(matrix);
	}

}