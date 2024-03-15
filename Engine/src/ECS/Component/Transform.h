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
	};
}

namespace CGEngine
{
	inline Math::Mat4 GetModelMatrix(const Component::Transform& transform)
	{
		const Math::Mat4 rotation = Math::ToMat4(Math::Quat(Math::DegToRad(transform.rotation)));

		return Math::Translate(Math::Mat4(1.0f), transform.position)
			   * rotation
			   * Math::Scale(Math::Mat4(1.0f), transform.scale);
	}
}