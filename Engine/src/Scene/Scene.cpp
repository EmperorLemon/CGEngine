#include "Scene.h"

#include "ECS/Entity/Entity.hpp"

#include "ECS/Component/DrawObject.h"

#include "Renderer/Assets/Model.h"
#include "IO/FileSystem.h"


namespace CGEngine
{
	Scene::Scene(const SceneCreateInfo& sceneInfo) : m_name(const_cast<std::string&>(sceneInfo.name)), m_entityList(const_cast<EntityList&>(sceneInfo.list))
	{
		m_camera.fov = GetFieldOfView(55.0f);

		Assets::Model model;
		IO::LoadModelFile("Assets/Models/Cube/cube.gltf", model);

		auto& ent = m_entityList.CreateEntity();
	}

}
