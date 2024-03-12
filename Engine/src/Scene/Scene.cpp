#include "Scene.h"

namespace CGEngine
{
	Scene::Scene(const SceneCreateInfo& sceneInfo) : m_name(const_cast<std::string&>(sceneInfo.name)), m_entityList(const_cast<EntityList&>(sceneInfo.list))
	{
		m_camera.fov = GetFieldOfView(55.0f);
	}

	const std::string& Scene::GetName() const
	{
		return m_name;
	}

	const Camera& Scene::GetMainCamera() const
	{
		return m_camera;
	}

}
