#pragma once

#include "Math/Math.h"
#include "Math/Transform.h"

#include "Math/Quaternion.h"

namespace CGEngine::Component
{
	struct Transform
	{
		Math::Vec3 position = Math::Vec3(0.0f);
		Math::Quat    rotation = Math::Quat(1.0f, Math::Vec3(0.0f)); // unit quaternion (no rotation) (x = 0, y = 0, z = 0, w = 1)
		Math::Vec3 scale    = Math::Vec3(1.0f);

		Math::Vec3 eulerAngles = Math::Vec3(0.0f);
	};
}

namespace CGEngine
{
	inline Math::Mat4 GetModelMatrix(const Component::Transform& transform)
	{
		auto model = Math::Mat4(1.0f);

		model =  Math::Translate(model, transform.position);

		
		model *= Math::ToMat4()

		model =  Math::Scale(model, transform.scale);

		return model;
	}
}