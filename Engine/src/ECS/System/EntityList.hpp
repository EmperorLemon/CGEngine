#pragma once

#include <entt/entity/registry.hpp>

namespace CGEngine
{
	class Entity;

	class EntityList
	{
	public:
		template <typename... Components, typename Func>
		void Iterate(Func&& func);

		template <typename Component, typename Func>
		void Sort(Func&& func);
	private:
		entt::registry m_registry;

		friend class Entity;
	};

	template <typename... Components, typename Func>
	void EntityList::Iterate(Func&& func)
	{
		m_registry.view<Components...>().each(std::forward<Func>(func));
	}

	template <typename Component, typename Func>
	void EntityList::Sort(Func&& func)
	{
		m_registry.sort<Component>(std::forward<Func>(func));
	}

}