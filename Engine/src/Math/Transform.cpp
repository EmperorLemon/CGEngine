#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CGEngine::Math
{
	Mat4 Perspective(const float fov, const float aspect, const float near, const float far)
	{
		return glm::perspective(fov, aspect, near, far);
	}

	Mat4 View(const Vec3& position, const Vec3& direction, const Vec3& up)
	{
		return lookAt(position, direction, up);
	}

	Mat4 Translate(const Mat4& transform, const Vec3& translation)
	{
		return translate(transform, translation);
	}

	Mat4 Rotate(const Mat4& transform, const float angle, const Vec3& axis)
	{
		return rotate(transform, angle, axis);
	}

	Mat4 Scale(const Mat4& transform, const Vec3& scale)
	{
		return glm::scale(transform, scale);
	}
}