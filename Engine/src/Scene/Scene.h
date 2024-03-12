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

		[[nodiscard]] const std::string& GetName() const  { return m_name; }
		[[nodiscard]] const Camera& GetMainCamera() const { return m_camera; }

		[[nodiscard]] const EntityList& GetEntities() const { return m_entityList; }
	private:
		std::string& m_name;
		Camera m_camera;

		EntityList& m_entityList;
	};
}
