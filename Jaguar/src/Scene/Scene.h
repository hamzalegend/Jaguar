#pragma once

#include "Entt/entt.h"

#include "API/Shader.h"
#include "API/Texture.h"
#include "API/Texture.h"
#include "Core/UUID.h"


class b2World;

class Entity;
class JAGUAR_API Scene
{
public:
	Scene();
	Scene(const Scene& other) {
	};
	~Scene();

	static Ref<Scene> copy(Ref<Scene> other);

	Entity CreateEntity(const std::string& name = "New Entity");
	Entity CreateEntityWithUUID(uint64_t uuid, const std::string& name = "New Entity");
	void DestroyEntity(Entity e);

	Entity GetEntityByUUID(Jaguar::UUID uuid);

	void OnUpdate(float deltaTime, bool RunPhysics = false);
	void clear();
	void OnRuntimeStart();
	void OnUpdateRuntime(float deltaTime);
	void OnRuntimeStop();

	Ref<Shader> shader;

private:

	void OnPhysicsStart();
	void OnPhysicsStop();

	entt::registry m_Registry;
	std::unordered_map<Jaguar::UUID, entt::entity> enttMap;

	b2World* m_physicsWorld;

	friend Entity;
	friend class SceneSerializer;
	friend class SceneHierarchyPanel;
	friend class GameView;
	friend class Renderer;
	friend class EditorLayer;
	friend class ScriptEngine;
};

