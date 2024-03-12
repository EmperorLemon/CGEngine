#include "SceneManager.h"

namespace CGEngine
{
	void SceneManager::AddScene(Scene&& scene)
	{
		m_scenes.emplace_back(std::move(scene));
	}

	Scene& SceneManager::DefaultScene()
	{
		return m_scenes.at(0);
	}
}