#pragma once

#include <string>

#include "Renderer/Camera.h"

#include "ECS/System/EntityList.hpp"

namespace CGEngine
{
	class Renderer;

	struct SceneCreateInfo
	{
		std::string name;
		EntityList list;
	};

	class Scene
	{
	public:
		explicit Scene(const SceneCreateInfo& sceneInfo);

		Scene(Scene&&) noexcept = default;
		Scene(const Scene&) noexcept = default;
		Scene& operator=(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) noexcept = delete;

		~Scene() = default;

		[[nodiscard]]  const std::string& GetName() const;
		[[nodiscard]] const Camera& GetMainCamera() const;
	private:
		std::string& m_name;
		Camera m_camera;

		EntityList& m_entityList;
	};
}
