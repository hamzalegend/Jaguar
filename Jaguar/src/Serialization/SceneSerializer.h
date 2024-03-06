#pragma once

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Core/DEFINES.h"
#include "glm/glm.hpp"
#include "Asset/AssetManager.h"

class JAGUAR_API SceneSerializer
{
public:
	SceneSerializer(Ref<Scene>& scene, Ref<AssetManager>assetManager);
	~SceneSerializer();

	void Serialize(const std::string& path);
	void DeSerialize(const std::string& path);

private:
	Ref<Scene>& m_scene;
	Ref<AssetManager> m_assetManager;
};
