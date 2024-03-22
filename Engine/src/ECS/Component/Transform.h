#pragma once

#include "Math/Math.h"
#include "Math/Transform.h"

#include "Math/Quaternion.h"

namespace CGEngine::Component
{
	struct Transform
	{
		Math::Vec3 position = Math::Vec3(0.0f);
		Math::Vec3 rotation = Math::Vec3(0.0f);
		Math::Vec3 scale    = Math::Vec3(1.0f);

		Math::Mat4 model    = Math::Mat4(1.0f);
	};
}

namespace CGEngine
{
	inline Math::Mat4 GetModelMatrix(const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
	{
		const Math::Mat4 rotation_matrix = Math::ToMat4(Math::Quat(Math::DegToRad(rotation)));

		return Math::Translate(Math::Mat4(1.0f), position)
			   * rotation_matrix
			   * Math::Scale(Math::Mat4(1.0f), scale);
	}
}