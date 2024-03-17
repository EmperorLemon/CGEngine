#include "EntityList.hpp"

#include "ECS/Entity/Entity.hpp"

namespace CGEngine
{
	std::vector<Entity> EntityList::m_entities = {};

	Entity& EntityList::CreateEntity(const std::string_view name, const uint8_t type)
	{
		m_entities.emplace_back(m_registry.create(), *this, name, type);

		return m_entities.back();
	}

	std::vector<Entity>& EntityList::GetEntities()
	{
		return m_entities;
	}

}