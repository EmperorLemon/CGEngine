#pragma once

#include <string>

#include "Renderer/Camera.h"

namespace CGEngine
{
	class Renderer;

	struct SceneCreateInfo
	{
		std::string name;
	};

	class Scene
	{
	public:
		explicit Scene(const SceneCreateInfo& sceneInfo);

		Scene(Scene&&) noexcept = delete;
		Scene& operator=(Scene&&) noexcept = delete;
		Scene(const Scene&) noexcept = delete;
		Scene& operator=(const Scene&) noexcept = delete;

		[[nodiscard]] const std::string& GetName() const;

		void SetupCamera();
	private:
		std::string& m_name;

		Camera m_camera;
	};
}
