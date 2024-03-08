#pragma once

#include <string>
#include <unordered_map>

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

		void AddScene(const Scene& scene);

		[[nodiscard]] const Scene& DefaultScene() const;
	private:
		std::unordered_map<std::string, Scene> m_scenes;
	};
}