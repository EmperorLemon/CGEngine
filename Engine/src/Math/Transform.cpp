#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CGEngine::Math
{
	Mat4 Perpsective(const float fov, const float aspect, const float near, const float far)
	{
		return glm::perspective(fov, aspect, near, far);
	}

	Mat4 View(const Vector3& position, const Vector3& direction, const Vector3& up)
	{
		return lookAt(position, direction, up);
	}

	Mat4 Translate(const Mat4& transform, const Vector3& translation)
	{
		return translate(transform, translation);
	}

	Mat4 Rotate(const Mat4& transform, const float angle, const Vector3& axis)
	{
		return rotate(transform, angle, axis);
	}

	Mat4 Scale(const Mat4& transform, const Vector3& scale)
	{
		return glm::scale(transform, scale);
	}
}