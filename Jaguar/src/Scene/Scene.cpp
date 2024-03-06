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

Ref<Scene> Scene::copy(Ref<Scene> other)
{
	Ref<Scene> newScene = MakeRef<Scene>();
	auto& src = other->m_Registry;
	for (auto e : other->m_Registry.view<TagComponent>())
	{
		newScene->CreateEntity(other->m_Registry.get<TagComponent>(e).name);
		Entity entity = { e, other.get() };

		if (src.any_of<TagComponent>(e))
			entity.AddComponent<TagComponent>().name = src.get<TagComponent>(e).name;
	}


	return newScene;
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity e = { m_Registry.create(), this };
	e.AddComponent<TagComponent>(name);
	e.AddComponent<TransformComponent>();
	return e;
}

void Scene::DestroyEntity(Entity e)
{
	m_Registry.destroy(e);
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
}

void Scene::clear()
{
	m_Registry.clear();
}

