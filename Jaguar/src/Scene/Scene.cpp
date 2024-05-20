#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include <iostream>

#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include <Renderer/Renderer.h>

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include "Components.h"

#include "ScriptableEntity.h"
#include <Scripting/ScriptEngine.h>


static b2BodyType RBtypeTob2DRBtype(RigidBody2DComponent::BodyType type)
{
	switch (type)
	{
	case RigidBody2DComponent::BodyType::Static: return b2_staticBody;
	case RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
	case RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
	}

	std::cout << "[error]unkown body type!! \n";
};

Scene::Scene()
{
	shader = MakeRef<Shader>("Assets/Shaders/basic.vs", "Assets/Shaders/basic.fs");
}



Scene::~Scene()
{

}
template<typename... Component>
static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<Jaguar::UUID, entt::entity>& enttMap)
{
	([&]()
		{
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = enttMap.at(src.get<UUIDComponent>(srcEntity).uuid);

				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
}

template<typename... Component>
static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<Jaguar::UUID, entt::entity>& enttMap)
{
	CopyComponent<Component...>(dst, src, enttMap);
}

template<typename... Component>
static void CopyComponentIfExists(Entity dst, Entity src)
{
	([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
}

template<typename... Component>
static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
{
	CopyComponentIfExists<Component...>(dst, src);
}

Ref<Scene> Scene::copy(Ref<Scene> other)
{
Ref<Scene> newScene = MakeRef<Scene>();

// newScene->m_ViewportWidth = other->m_ViewportWidth;
// newScene->m_ViewportHeight = other->m_ViewportHeight;

auto& srcSceneRegistry = other->m_Registry;
auto& dstSceneRegistry = newScene->m_Registry;
std::unordered_map<Jaguar::UUID, entt::entity> enttMap;

// Create entities in new scene
auto idView = srcSceneRegistry.view<UUIDComponent>();
for (auto e : idView)
{
	Jaguar::UUID uuid = srcSceneRegistry.get<UUIDComponent>(e).uuid;
	const auto& name = srcSceneRegistry.get<TagComponent>(e).name;
	Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
	enttMap[uuid] = (entt::entity)newEntity;
}

// Copy components (except IDComponent and TagComponent)
CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

return newScene;
}
Entity Scene::CreateEntity(const std::string& name)
{
	Entity e = { m_Registry.create(), this };
	e.AddComponent<TagComponent>(name);
	enttMap[e.AddComponent<UUIDComponent>().uuid] = e;
	e.AddComponent<TransformComponent>();
	return e;
}

Entity Scene::CreateEntityWithUUID(uint64_t uuid, const std::string& name)
{
	Entity e = { m_Registry.create(), this };
	e.AddComponent<TagComponent>(name);
	enttMap[uuid] = e;
	e.AddComponent<UUIDComponent>().uuid = Jaguar::UUID(uuid);
	e.AddComponent<TransformComponent>();
	return e;
// 	return Entity();
}


void Scene::DestroyEntity(Entity e)
{
	enttMap.erase(e.GetComponent<UUIDComponent>().uuid);
	m_Registry.destroy(e);
}

Entity Scene::GetEntityByUUID(Jaguar::UUID uuid)
{
	return { enttMap[uuid] , this};
}

void Scene::OnPhysicsStart()
{
	m_physicsWorld = new b2World({ 0.0f, -9.8f });

	auto view = m_Registry.view<RigidBody2DComponent>();
	for (auto e : view)
	{
		Entity entity = { e, this };
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = RBtypeTob2DRBtype(rb2d.Type);
		bodyDef.position.Set(transform.Position.x, transform.Position.y);
		bodyDef.angle = transform.Rotation.z;

		b2Body* body = m_physicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.FixedRotation);
		rb2d.RunTimeBody = body;

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

			b2PolygonShape boxShape;
			boxShape.SetAsBox(bc2d.size.x * transform.Scale.x, bc2d.size.y * transform.Scale.y, b2Vec2(bc2d.offset.x, bc2d.offset.y), 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = bc2d.Density;
			fixtureDef.friction = bc2d.Friction;
			fixtureDef.restitution = bc2d.Restitution;
			fixtureDef.restitutionThreshold = bc2d.RestitutionThreshhold;
			body->CreateFixture(&fixtureDef);
		}
	}

}

void Scene::OnPhysicsStop()
{
	delete m_physicsWorld;
}


void Scene::OnUpdate(float deltaTime, bool RunPhysics)
{
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(deltaTime);
			});
	}

	// Physics:
	if (RunPhysics)
	{
		const int32_t velocityItterations = 6;
		const int32_t positionItterations = 2;
		m_physicsWorld->Step(deltaTime, velocityItterations, positionItterations);

		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RunTimeBody;
			const auto& position = body->GetPosition();
			transform.Position.x = position.x;
			transform.Position.y = position.y;

			transform.Rotation.z = body->GetAngle();

		}
	}

	// Renderring:
	{

		Entity mainCam{ (entt::entity)-1, this };
		auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			const auto& transform = group.get<TransformComponent>(entity);
			if (Entity{ entity, this }.GetComponent<CameraComponent>().Primary)
			{
				if (group.get<CameraComponent>(entity).Primary)
				{
					mainCam = { entity, this };
				}

			}
		}
		if (mainCam != -1)
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // TODO:: remove
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO:: remove
			Renderer::RednerScene(this, &mainCam.GetComponent<CameraComponent>().camera, inverse(mainCam.GetComponent<TransformComponent>().GetTransform()));
		}
	}

	if (RunPhysics) OnUpdateRuntime(deltaTime);
}

void Scene::clear()
{
	m_Registry.clear();
}

void Scene::OnRuntimeStart()
{
	OnPhysicsStart();

	ScriptEngine::OnRuntimeStart(this);

	auto view = m_Registry.view<ScriptComponent>();
	for (auto e : view)
	{
		Entity entity = { e, this };

		ScriptEngine::OnCreateEntity(entity);
		
	}
}

void Scene::OnUpdateRuntime(float deltaTime)
{
	// ScriptEngine::OnRuntimeStop(); // should this be here? ... i truly dont know

	auto view = m_Registry.view<ScriptComponent>();
	for (auto e : view)
	{
		Entity entity = { e, this };

		ScriptEngine::OnUpdateEntity(entity, deltaTime);

	}

}

void Scene::OnRuntimeStop()
{
	ScriptEngine::OnRuntimeStop();
	OnPhysicsStop();
}

