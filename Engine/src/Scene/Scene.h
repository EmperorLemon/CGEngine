#pragma once

#include <string>

#include "Renderer/Camera.h"

#include "ECS/System/EntityList.hpp"

namespace CGEngine
{
	class Renderer;

	class Scene
	{
	public:
		explicit Scene(const std::string& name);

		void SetupScene();

		[[nodiscard]] const std::string& GetName() const  { return m_name; }

		[[nodiscard]] Camera& GetMainCamera() { return m_camera; }
		[[nodiscard]] EntityList& GetEntities() const { return *m_entityList; }
	private:
		std::string& m_name;
		Camera m_camera;

		std::unique_ptr<EntityList> m_entityList = nullptr;
	};
}
