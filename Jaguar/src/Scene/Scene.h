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

	void OnUpdate(float deltaTime, bool RunPhysics = false);
	void clear();

	void OnPhysicsStart();
	void OnPhysicsStop();

	Ref<Shader> shader;
	// Ref<Texture> texture1;


private:
	entt::registry m_Registry;

	b2World* m_physicsWorld;

	friend Entity;
	friend class SceneSerializer;
	friend class SceneHierarchyPanel;
	friend class GameView;
	friend class Renderer;
	friend class EditorLayer;
};

