#include "Entity.hpp"

#include "ECS/Component/Transform.h"
#include "ECS/Component/Tag.h"

namespace CGEngine
{
	Entity::Entity(const entt::entity handle, EntityList& list, const std::string_view name) : m_handle(handle), m_list(list)
	{
		this->AddComponent<Component::Tag>(Component::Tag(name.data(), Utils::GUID()));
		this->AddComponent<Component::Transform>();
	}
}
