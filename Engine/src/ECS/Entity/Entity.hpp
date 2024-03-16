#pragma once

#include "Core/Logger.hpp"

#include <entt/entity/entity.hpp>

#include <stdexcept>
#include <string_view>

#include "ECS/System/EntityList.hpp"

namespace CGEngine
{
	class Entity
	{
	public:
		Entity(entt::entity handle, EntityList& list, std::string_view name);

		template <typename... Types>
		[[nodiscard]] bool HasComponent() const;

		template <class T, typename... Args>
		void AddComponent(Args&&... args);

		template <class T>
		T& GetComponent() const;
	private:
		entt::entity m_handle = { entt::null };
		EntityList& m_list;

		friend EntityList;
	};

	template <typename... Types>
	bool Entity::HasComponent() const
	{
		return m_list.m_registry.all_of<Types...>(m_handle);
	}

	template <class T, typename... Args>
	void Entity::AddComponent(Args&&... args)
	{
		if (this->HasComponent<T>())
			CG_WARN("Entity already has component!");

		m_list.m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
	}

	template <class T>
	T& Entity::GetComponent() const
	{
		if (!this->HasComponent<T>())
			throw std::runtime_error("Unable to fetch component!");

		return m_list.m_registry.get<T>(m_handle);
	}
}
