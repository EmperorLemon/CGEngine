#pragma once

#include <entt/entity/entity.hpp>

namespace CGEngine
{
	class Entity
	{
	public:
		Entity() = default;

		template <typename... Types>
		[[nodiscard]] bool HasComponent() const;
	private:
		entt::entity m_handle = { entt::null };
	};

	template <typename... Types>
	bool Entity::HasComponent() const
	{
		return true;
	}
}