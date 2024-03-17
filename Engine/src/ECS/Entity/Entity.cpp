#include "Entity.hpp"

#include "ECS/Component/Transform.h"
#include "ECS/Component/Tag.h"

namespace CGEngine
{
	Entity::Entity(const entt::entity handle, EntityList& list, const std::string_view name, const uint8_t type) : m_handle(handle), m_list(list)
	{
		this->AddComponent<Component::Tag>(Component::Tag(name.data(), static_cast<Component::EntityType>(type), Utils::GUID()));
		this->AddComponent<Component::Transform>();
	}
}
