#include "SceneSerializer.h"

#include <fstream>

#include <Scene/Components.h>
#include <Scene/Scene.h>
#include <core/DEFINES.h>
#include <Renderer/ModelLoader.h>

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
	
		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
	
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};


	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

void SerializeEntity(YAML::Emitter& out, Entity e)
{
	out << YAML::BeginMap;
	out << YAML::Key << "Entity" << YAML::Value << "065412306846"; // GUID goes into value

	if (e.HasComponent<TagComponent>())
	{
		auto& component = e.GetComponent<TagComponent>();
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap; // TagComponent

		out << YAML::Key << "Tag" << YAML::Value << component.name.c_str();

		out << YAML::EndMap; // TagComponent
	}

	if (e.HasComponent<TransformComponent>())
	{
		auto& component = e.GetComponent<TransformComponent>();
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap; // TransformComponent

		out << YAML::Key << "Position" << YAML::Value << component.Position;
		out << YAML::Key << "Rotation" << YAML::Value << component.Rotation;
		out << YAML::Key << "Scale" << YAML::Value << component.Scale;

		out << YAML::EndMap; // TransformComponent
	}


	if (e.HasComponent<SpriteRendererComponent>())
	{
		auto& component = e.GetComponent<SpriteRendererComponent>();
		out << YAML::Key << "SpriteRendererComponent";
		out << YAML::BeginMap; // SpriteRendererComponent

		out << YAML::Key << "Color" << YAML::Value << component.sprite.color;
		out << YAML::Key << "texturePath" << YAML::Value << component.sprite.texturePath;

		out << YAML::EndMap; // SpriteRendererComponent
	}

	if (e.HasComponent<MeshRendererComponent>())
	{
		auto& component = e.GetComponent<MeshRendererComponent>();
		out << YAML::Key << "MeshRendererComponent";
		out << YAML::BeginMap; // MeshRendererComponent

		out << YAML::Key << "meshPath" << YAML::Value << component.mesh.meshPath;
		out << YAML::Key << "MaterialPath" << YAML::Value << component.mesh.Material->Path;

		out << YAML::EndMap; // MeshRendererComponent
	}

	if (e.HasComponent<NativeScriptComponent>())
	{

	}

	if (e.HasComponent<CameraComponent>())
	{
		auto& component = e.GetComponent<CameraComponent>();
		out << YAML::Key << "CameraComponent";
		out << YAML::BeginMap; // CameraComponent

		out << YAML::Key << "Primary" << YAML::Value << component.Primary;
		out << YAML::Key << "FOV" << YAML::Value << component.camera.FOV;
		out << YAML::Key << "NearPlane" << YAML::Value << component.camera.NearPlane;
		out << YAML::Key << "FarPlane" << YAML::Value << component.camera.FarPlane;

		out << YAML::EndMap; // CameraComponent
	}

	if (e.HasComponent<RigidBody2DComponent>())
	{
		auto& component = e.GetComponent<RigidBody2DComponent>();
		out << YAML::Key << "RigidBody2DComponent";
		out << YAML::BeginMap; // RigidBody2DComponent

		out << YAML::Key << "Type" << YAML::Value << (int)component.Type;
		out << YAML::Key << "FixedRotation" << YAML::Value << component.FixedRotation;

		out << YAML::EndMap; // RigidBody2DComponent
	}

	if (e.HasComponent<BoxCollider2DComponent>())
	{
		auto& component = e.GetComponent<BoxCollider2DComponent>();
		out << YAML::Key << "BoxCollider2DComponent";
		out << YAML::BeginMap; // BoxCollider2DComponent

		out << YAML::Key << "offset" << YAML::Value << component.offset;
		out << YAML::Key << "size" << YAML::Value << component.size;

		out << YAML::Key << "Density" << YAML::Value << component.Density;
		out << YAML::Key << "Friction" << YAML::Value << component.Friction;
		out << YAML::Key << "Restitution" << YAML::Value << component.Restitution;
		out << YAML::Key << "RestitutionThreshhold" << YAML::Value << component.RestitutionThreshhold;

		out << YAML::EndMap; // BoxCollider2DComponent
	}

	if (e.HasComponent<lightComponent>())
	{
		auto& component = e.GetComponent<lightComponent>();
		out << YAML::Key << "lightComponent";
		out << YAML::BeginMap; // light

		out << YAML::Key << "Color" << YAML::Value << component.Color;
		out << YAML::Key << "Intensity" << YAML::Value << component.Intensity;
		out << YAML::Key << "Type" << YAML::Value << (int)component.Type;

		out << YAML::EndMap; // light
	}

	out << YAML::EndMap; //Entity
}

SceneSerializer::SceneSerializer(Ref<Scene>& scene, Ref<AssetManager> assetManager)
	:m_scene(scene), m_assetManager(assetManager)
{

}

SceneSerializer::~SceneSerializer()
{
}

void SceneSerializer::Serialize(const std::string& path)
{
	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";

	out << YAML::Key << "Entites" << YAML::BeginSeq;
	m_scene->m_Registry.view<TagComponent>().each([&](auto entityID, auto& _)
		{
			Entity e = { entityID, m_scene.get() };
			if (e == -1) return;

			SerializeEntity(out, e);
		});

	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(path);
	fout << out.c_str();
}

void SceneSerializer::DeSerialize(const std::string& path)
{
	m_scene = MakeRef<Scene>();

	YAML::Node data;
	try
	{
		data = YAML::LoadFile(path);
	}
	catch (YAML::ParserException e)
	{
		// HZ_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", filepath, e.what());
		return;
	}

	if (!data["Scene"])
		return;

	std::string sceneName = data["Scene"].as<std::string>();

	auto entities = data["Entites"];

	if (entities)
	{
		for (auto entity : entities)
		{
			// ID
			// uint32_t uuid = entity["Entity"].as<uint32_t>();

			// Tag
			std::string name;
			auto TagComponent = entity["TagComponent"];
			if (TagComponent)
				name = TagComponent["Tag"].as<std::string>();
			auto e = m_scene->CreateEntity(name);

			// Transform
			auto transform = entity["TransformComponent"];
			TransformComponent& tc = e.GetComponent<TransformComponent>();
			tc.Position = transform["Position"].as<glm::vec3>();
			tc.Rotation = transform["Rotation"].as<glm::vec3>();
			tc.Scale = transform["Scale"].as<glm::vec3>();

			// Camera
			auto camera = entity["CameraComponent"];
			if (camera)
			{
				CameraComponent& CC = e.AddComponent<CameraComponent>();
				CC.Primary = camera["Primary"].as<bool>();
				CC.camera.FOV = camera["FOV"].as<float>();
				CC.camera.NearPlane = camera["NearPlane"].as<float>();
				CC.camera.FarPlane = camera["FarPlane"].as<float>();
				CC.camera.SetProjection(glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f));
			}

			// Camera
			auto sprite = entity["SpriteRendererComponent"];
			if (sprite)
			{
				SpriteRendererComponent& SRC = e.AddComponent<SpriteRendererComponent>();
				SRC.sprite.color = sprite["Color"].as<glm::vec4>();
				SRC.sprite.texturePath = sprite["texturePath"].as<std::string>();
				SRC.sprite.texture = MakeRef<Texture>(SRC.sprite.texturePath.c_str());

			}
			// MeshRenderer
			auto mesh = entity["MeshRendererComponent"];
			if (mesh)
			{
				ModelManager mm;

				MeshRendererComponent& MRC = e.AddComponent<MeshRendererComponent>();
				MRC.mesh.meshPath = mesh["meshPath"].as<std::string>();
				MRC.mesh.Material = AssetManager::LoadMaterial(mesh["MaterialPath"].as<std::string>().c_str());
				// MRC.mesh.texture = MakeRef<Texture>(MRC.mesh.texturePath.c_str());
				mm.loadModel(MRC.mesh.meshPath);
				*MRC.mesh.vertices = mm.getVertexData();
				*MRC.mesh.indices = mm.getIndexData();
				mm.clear();
				MRC.ReBuild();
			}

			// RigidBody2D
			auto RB2D = entity["RigidBody2DComponent"];
			if (RB2D)
			{
				RigidBody2DComponent& RB = e.AddComponent<RigidBody2DComponent>();
				RB.Type = (RigidBody2DComponent::BodyType)RB2D["Type"].as<int>();
				RB.FixedRotation = RB2D["FixedRotation"].as<bool>();
			}

			// BoxCollider2DComponent
			auto BC2D = entity["BoxCollider2DComponent"];
			if (BC2D)
			{
				BoxCollider2DComponent& BC = e.AddComponent<BoxCollider2DComponent>();
				BC.size = BC2D["size"].as<glm::vec2>();
				BC.offset = BC2D["offset"].as<glm::vec2>();
				BC.Density = BC2D["Density"].as<float>();
				BC.Friction = BC2D["Friction"].as<float>();
				BC.Restitution = BC2D["Restitution"].as<float>();
				BC.RestitutionThreshhold = BC2D["RestitutionThreshhold"].as<float>();
			}

			// Light
			auto Light = entity["lightComponent"];
			if (Light)
			{
				lightComponent& LC = e.AddComponent<lightComponent>();
				LC.Color = Light["Color"].as<glm::vec4>();
				LC.Intensity = Light["Intensity"].as<float>();
				LC.Type = (lightComponent::LightType)Light["Type"].as<int>();
			}

		}
	}
	std::cout << path.c_str() << "\n";

}