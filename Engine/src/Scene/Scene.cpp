#include "Scene.h"

#include "ECS/Entity/Entity.hpp"

#include "ECS/Component/Transform.h"
#include "ECS/Component/DrawObject.h"

#include "Renderer/Buffer.h"

#include "Renderer/Assets/Model.h"
#include "IO/FileSystem.h"


namespace CGEngine
{
	constexpr size_t AMOUNT = 10;

	std::vector<Math::Mat4> matrices;

	Scene::Scene(const std::string& name) : m_name(const_cast<std::string&>(name)), m_entityList(std::make_unique<EntityList>())
	{
		m_camera.fov = GetFieldOfView(55.0f);
	}

	void Scene::SetupScene() 
	{
		auto offset = Math::Vec3(1.0f);

		for (size_t i = 0; i < AMOUNT; ++i)
		{
			auto mdl = Math::Mat4(1.0f);

			offset.x = 2.0f * static_cast<float>(i);

			mdl = Math::Translate(mdl, offset);

			mdl = Math::Rotate(mdl, 0.0f, Math::X_AXIS);
			mdl = Math::Rotate(mdl, 0.0f, Math::Y_AXIS);
			mdl = Math::Rotate(mdl, 0.0f, Math::Z_AXIS);

			mdl = Math::Scale(mdl, Math::Vec3(1.0f));

			matrices.emplace_back(mdl);
		}

		Assets::Model model;
		IO::LoadModelFile("Assets/Models/Cube/cube.gltf", model);

		auto& entity = m_entityList->CreateEntity();
		entity.AddComponent<Component::DrawObject>(std::move(model));

		const auto& drawObject = entity.GetComponent<Component::DrawObject>();

		for (const auto& vertexArray : drawObject.vertexArrays)
		{
			vertexArray->SetDrawType(DrawType::DRAW_ELEMENTS_INSTANCED);
			vertexArray->SetupInstancing(5, AMOUNT, matrices.data());
		}

		for (const auto& matrix : matrices)
		{
			const auto& positon = Math::Vec3(matrix[3][0], matrix[3][1], matrix[3][2]);

			CG_TRACE("{0} {1} {2}", positon.x, positon.y, positon.z);
		}
	}
}
