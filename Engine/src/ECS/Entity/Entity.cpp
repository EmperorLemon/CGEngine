#include "Entity.hpp"

#include "ECS/Component/Transform.h"
#include "Utils/GUID.h"

namespace CGEngine
{
	Entity::Entity(const entt::entity handle, EntityList& list) : m_handle(handle), m_list(list)
	{
		this->AddComponent<Utils::GUID>();
		this->AddComponent<Component::Transform>();
	}
}
