#include "Scene.h"

#include "Core/Time.h"

#include "ECS/Entity/Entity.hpp"

#include "ECS/Component/DrawObject.h"
#include "ECS/Component/Instance.h"
#include "ECS/Component/Light.h"
#include "ECS/Component/Transform.h"

#include "Renderer/Buffer.h"

#include "Renderer/Assets/Model.h"
#include "IO/FileSystem.h"

constexpr int32_t INSTANCE_COUNT = 15;
constexpr int32_t    LIGHT_COUNT = 1;

namespace CGEngine
{
	Scene::Scene(const std::string& name) : m_name(const_cast<std::string&>(name)), m_entityList(std::make_unique<EntityList>())
	{
		m_camera.fov = GetFieldOfView(55.0f);
	}

	void Scene::SetupScene() 
	{
		{
			Assets::Model model;
			IO::LoadModelFile("Assets/Models/Cube/cube.gltf", model);

			auto& entity = m_entityList->CreateEntity("Cube", static_cast<uint8_t>(Component::EntityType::DRAWOBJECT));
			entity.AddComponent<Component::Instance>(0);
			entity.AddComponent<Component::DrawObject>(std::move(model));

			const auto& drawObject = entity.GetComponent<Component::DrawObject>();
			drawObject.vertexArrays.at(0)->SetDrawType(DrawType::DRAW_ELEMENTS_INSTANCED);
			drawObject.vertexArrays.at(0)->SetInstanceCount(INSTANCE_COUNT);

			entity.GetComponent<Component::Transform>().position = Math::Vec3(0.0f, 0.0f, 0.0f);
		}

		for (int32_t i = 0; i < INSTANCE_COUNT - 1; ++i)
		{
			auto& entity = m_entityList->CreateEntity("Cube Instance " + std::to_string(i + 1), static_cast<uint8_t>(Component::EntityType::INSTANCE));
			entity.AddComponent<Component::Instance>(i);

			entity.GetComponent<Component::Transform>().position = Math::Vec3(0.0f, 2.10 * (i + 1), 0.0f);
		}

		for (int32_t i = 0; i < LIGHT_COUNT; ++i)
		{
			auto& entity = m_entityList->CreateEntity("Light " + std::to_string(i), static_cast<uint8_t>(Component::EntityType::LIGHT));
			entity.AddComponent<Component::Light>();

			entity.GetComponent<Component::Transform>().position = Math::Vec3(-2.0f, 4.0f, -1.0f);
		}
	}

	void Scene::Update(const Time& time)
	{

	}
}
