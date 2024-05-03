#pragma once
#include <iostream>
#define JR_CORE_ASSERT(x, y) {if(x)std::cout << y << "\n";}

#include "Scene.h"
#include "Entt/entt.h"
// #include "Jaguar/Log/log.h"
#include "Components.h"
#include "Core/UUID.h"
class Entity
{
public:
	Entity() = default;
	Entity(const Entity& other) = default;
	Entity(entt::entity EntityHandle, Scene* Scene)
		:m_EntityHandle(EntityHandle), m_Scene(Scene)
	{
	}

	~Entity() {};

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		JR_CORE_ASSERT(HasComponent<T>(), "Alraedy has Component!");
		if (!HasComponent<T>())
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		else
			return GetComponent<T>();
	}

	template<typename T>
	T& GetComponent()
	{
		JR_CORE_ASSERT(!HasComponent<T>(), "Doesnt Have Component!");
		if (HasComponent<T>())
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		// return m_Scene->m_Registry.has // doesnt Work
		return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
	}

	template<typename T>
	bool RemoveComponent()
	{
		JR_CORE_ASSERT(!HasComponent<T>(), "Doesnt Have Component!");
		return m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	// Jaguar::UUID GetUUID() { return GetComponent<UUIDComponent>().m_UUID; }

	operator uint32_t() const {
		return (uint32_t)m_EntityHandle;
	}

	bool operator==(const Entity other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
	bool operator!=(const Entity other) const { return !(*this == other); }
	operator entt::entity() const { return m_EntityHandle; }
private:
	entt::entity m_EntityHandle{ entt::null };
	Scene* m_Scene;
};
