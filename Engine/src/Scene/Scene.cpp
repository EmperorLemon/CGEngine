#include "Scene.h"

#include "ECS/Entity/Entity.hpp"

#include "ECS/Component/Transform.h"
#include "ECS/Component/DrawObject.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

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
		IO::LoadModelFile("Assets/Models/Cube/cube.gltf", model);

		auto& entity = m_entityList->CreateEntity();
		entity.AddComponent<Component::DrawObject>(std::move(model));

		const auto& drawObject = entity.GetComponent<Component::DrawObject>();
		drawObject.vertexArrays.at(0)->SetDrawType(DrawType::DRAW_ELEMENTS_INSTANCED);
		drawObject.vertexArrays.at(0)->SetInstanceCount(1);
	}
}
