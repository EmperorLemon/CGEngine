#include "Scene.h"

#include "Math/Transform.h"

namespace CGEngine
{
	Scene::Scene(const SceneCreateInfo& sceneInfo) : m_name(const_cast<std::string&>(sceneInfo.name))
	{
		SetupCamera();
	}

	void Scene::SetupCamera()
	{
		m_camera.fov = GetFieldOfView(55.0f);

		m_camera.projection = Math::Perspective(m_camera.fov, m_camera.aspect, m_camera.near, m_camera.far);
		m_camera.view = Math::View(m_camera.position, m_camera.direction, m_camera.up);
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
