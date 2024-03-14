#include "Scene.h"

#include "ECS/Entity/Entity.hpp"

#include "ECS/Component/Transform.h"
#include "ECS/Component/DrawObject.h"

#include "Renderer/Assets/Model.h"
#include "IO/FileSystem.h"


namespace CGEngine
{
	Scene::Scene(const std::string& name) : m_name(const_cast<std::string&>(name)), m_entityList(std::make_unique<EntityList>())
	{
		m_camera.fov = GetFieldOfView(55.0f);
	}

	void Scene::SetupScene() 
	{
		Assets::Model model;
		IO::LoadModelFile("Assets/Models/Cube/Cube.gltf", model);

		auto& ent = m_entityList->CreateEntity();
		ent.AddComponent<Component::DrawObject>(std::move(model));
	}
}
