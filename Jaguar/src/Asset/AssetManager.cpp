#include "AssetManager.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>


#include <fstream>

static std::filesystem::path s_ProjectPath;

AssetManager::AssetManager()
{

}

void AssetManager::Init(std::filesystem::path projectPath)
{
	s_ProjectPath = projectPath;
}

AssetManager::~AssetManager()
{
}
Ref<Material> AssetManager::LoadMaterial(const char* path)
{
	Ref<Material> m_mat = MakeRef<Material>();

	YAML::Node data;
	try
	{
		data = YAML::LoadFile((s_ProjectPath/ path).string().c_str());
	}
	catch (YAML::ParserException e)
	{
		return nullptr;
	}

	if (!data["Material"])
		return nullptr;


	std::string materialName = data["Material"].as<std::string>();

	m_mat->VertexShaderPath = data["VertexShaderPath"].as<std::string>();
	m_mat->FragmentShaderPath = data["FragmentShaderPath"].as<std::string>();
	m_mat->AlbedoPath= data["AlbedoPath"].as<std::string>();

	m_mat->Shader = MakeRef<Shader>((s_ProjectPath / m_mat->VertexShaderPath).string().c_str(), (s_ProjectPath / m_mat->FragmentShaderPath).string().c_str());
	m_mat->Albedo = MakeRef<Texture>((s_ProjectPath / m_mat->AlbedoPath).string().c_str());

	m_mat->Path = path;

	return m_mat;
}

void AssetManager::SaveMaterial(std::filesystem::path path, Ref<Material>& material)
{
	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "Material" << YAML::Value << path.string().c_str();
	
	//
	out << YAML::Key << "VertexShaderPath" << YAML::Value << material->VertexShaderPath;
	out << YAML::Key << "FragmentShaderPath" << YAML::Value << material->FragmentShaderPath;
	out << YAML::Key << "AlbedoPath" << YAML::Value << material->AlbedoPath;
	//
	out << YAML::EndMap;

	std::ofstream fout(material->Path.c_str());
	fout << out.c_str();
}
/*
int AssetManager::LoadTexture(std::string path)
{
	int ID = GenID();
	m_Savemap.insert({ ID, path });
	m_Runmtimemap.insert({ ID, MakeRef<Texture>(path.c_str()) });
	return ID;
}

Ref<Texture> AssetManager::GetTexture(int ID)
{
	if (m_Runmtimemap.find(ID) != m_Runmtimemap.end())
		return m_Runmtimemap[ID];
}
template <typename T>
int AssetManager::Load(std::string path)
{
	int ID = GenID();
	m_Savemap.insert({ ID, path });
	m_Runmtimemap.insert({ ID, MakeRef<T>(path.c_str()) });
	return ID;
}

template <typename T>
Ref<T> AssetManager::Get(int ID)
{
	if (m_Runmtimemap.find(ID) != m_Runmtimemap.end())
		return std::any_cast<T>(m_Runmtimemap[ID]);
	// return Ref<std::any>();
}

*/