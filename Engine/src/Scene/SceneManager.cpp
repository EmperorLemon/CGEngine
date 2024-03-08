#include "SceneManager.h"

namespace CGEngine
{
	void SceneManager::AddScene(const Scene& scene)
	{
		if (const auto& sceneName = scene.GetName(); !m_scenes.contains(sceneName))
			m_scenes.insert(std::make_pair(sceneName, scene));
	}

	const Scene& SceneManager::DefaultScene() const
	{
		return m_scenes.at("Default Scene");
	}
}