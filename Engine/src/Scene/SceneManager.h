#pragma once

#include <vector>

#include "Scene.h"

namespace CGEngine::Utils
{
	class GUID;
}

namespace CGEngine
{
	class SceneManager
	{
	public:
		SceneManager() = default;

		void AddScene(Scene&& scene);

		[[nodiscard]] Scene& DefaultScene();
	private:
		std::vector<Scene> m_scenes;
	};
}