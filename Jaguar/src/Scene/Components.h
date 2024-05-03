#pragma once

#include "Renderer/Mesh.h"

// #include "ScriptableEntity.h"

#include "Camera/Camera.h"
#include <Core/UUID.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>

class ScriptableEntity;

// struct UUIDComponent
// {
// 	// Jaguar::UUID GetUUID() { return m_UUID; }
// 	uint32_t UUID;
// private:
// 
// 	UUIDComponent() = default;
// 	UUIDComponent(const UUIDComponent&) = default;
// };

struct TransformComponent
{
	glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	glm::mat4 GetTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

		return glm::translate(glm::mat4(1.0f), Position)
			* rotation
			* glm::scale(glm::mat4(1.0f), Scale);
	}


	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& translation)
		: Position(translation) {}
};
struct TagComponent
{
	std::string name;
	operator std::string& () { return name; }


	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
};


struct UUIDComponent
{
	std::string name;
	operator std::string& () { return name; }

	Jaguar::UUID uuid;
	UUIDComponent() = default;
	UUIDComponent(const UUIDComponent&) = default;
};

struct MeshRendererComponent
{
	Mesh mesh;

	void ReBuild()
	{
		vb = MakeRef<VertexBuffer>(*mesh.vertices);
		va = MakeRef<VertexArray>();
		va->Bind();
		va->Addlayout(3); // Positions
		va->Addlayout(3); // Normals
		va->Addlayout(2); // TexCoords
		va->Flush();


		ib = MakeRef<IndexBuffer>(*mesh.indices);

	}
	Ref<VertexBuffer> vb;
	Ref<VertexArray> va;
	Ref<IndexBuffer> ib;


	MeshRendererComponent() = default;
	MeshRendererComponent(const MeshRendererComponent&) = default;
};

struct SpriteRendererComponent
{
	Sprite sprite;

	SpriteRendererComponent() = default;
	SpriteRendererComponent(const SpriteRendererComponent&) = default;
};

struct CameraComponent
{
	Camera camera;
	bool Primary = true;

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
};

struct ScriptComponent
{
	std::string ClassName;

	ScriptComponent() = default;
	ScriptComponent(const ScriptComponent&) = default;
};

struct NativeScriptComponent
{
	ScriptableEntity* Instance = nullptr;

	ScriptableEntity* (*InstantiateScript)();
	void (*DestroyScript)(NativeScriptComponent*);

	template<typename T>
	void Bind()
	{
		InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
	}

	NativeScriptComponent() = default;
	NativeScriptComponent(const NativeScriptComponent&) = default;
};

struct RigidBody2DComponent
{
	enum class BodyType
	{
		Static = 0,
		Dynamic,
		Kinematic,
	};

	// RigidBody2DComponent(BodyType type = BodyType::Static) :Type(type) {};
	// RigidBody2DComponent() = default;
	// RigidBody2DComponent(RigidBody2DComponent&) = default;

	BodyType Type;
	bool FixedRotation = false;

	void* RunTimeBody = nullptr;

	RigidBody2DComponent() = default;
	RigidBody2DComponent(const RigidBody2DComponent&) = default;
};

struct BoxCollider2DComponent
{
	// BoxCollider2DComponent() = default;
	// BoxCollider2DComponent(BoxCollider2DComponent&) = default;

	glm::vec2 offset = { 0.0f, 0.0f };
	glm::vec2 size = { 0.5f, 0.5f };

	// TODO:: to Physics mats:
	float Density = 1;
	float Friction = 0.5f;
	float Restitution = 0.0f;
	float RestitutionThreshhold = 0.5f;
	//

	void* RunTimeFixture = nullptr;

	BoxCollider2DComponent() = default;
	BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
};

struct lightComponent
{
	enum class LightType
	{
		Directional = 0,
		Point,
		Spot,
	};

	LightType Type = LightType::Directional;
	glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float Intensity = 1;

	// point && spot
	float constant = 1.0f;
	float linear = 0.009f;
	float quadratic = 1.032f;

	// spot
	float CutOff = 12;
	float OuterCutOff = 20;

	lightComponent() = default;
	lightComponent(const lightComponent&) = default;
};
