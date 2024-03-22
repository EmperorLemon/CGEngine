#pragma once

#include <vector>
#include <entt/entity/registry.hpp>

namespace CGEngine
{
	class Entity;

	class EntityList
	{
	public:
		EntityList() = default;

		Entity& CreateEntity(std::string_view name, uint8_t type);

		template <typename... Components, typename Func>
		void Iterate(Func&& func);

		template <typename Component, typename Func>
		void Sort(Func&& func);

		template <typename Component>
		[[nodiscard]] size_t Count() const;

		static std::vector<Entity>& GetEntities();
	private:
		entt::registry m_registry;
		static std::vector<Entity> m_entities;

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

	template <typename Component>
	size_t EntityList::Count() const
	{
		return m_registry.view<Component>().size();
	}

}