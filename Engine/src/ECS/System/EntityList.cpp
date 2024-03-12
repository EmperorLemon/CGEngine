#include "EntityList.hpp"

#include "ECS/Entity/Entity.hpp"

namespace CGEngine
{
	std::vector<Entity> EntityList::m_entities = {};

	Entity& EntityList::CreateEntity()
	{
		m_entities.emplace_back(m_registry.create(), *this);

		return m_entities.back();
	}
}