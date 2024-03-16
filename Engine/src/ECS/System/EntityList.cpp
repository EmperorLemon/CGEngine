#include "EntityList.hpp"

#include "ECS/Entity/Entity.hpp"

namespace CGEngine
{
	std::vector<Entity> EntityList::m_entities = {};

	Entity& EntityList::CreateEntity(const std::string_view name)
	{
		m_entities.emplace_back(m_registry.create(), *this, name);

		return m_entities.back();
	}

	std::vector<Entity>& EntityList::GetEntities()
	{
		return m_entities;
	}

}