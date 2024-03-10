#pragma once

#include <glm/mat4x4.hpp>

namespace CGEngine::Math
{
	using Mat4 = glm::mat4;

	Mat4 Transpose(const Mat4& matrix);
	Mat4   Inverse(const Mat4& matrix);
}