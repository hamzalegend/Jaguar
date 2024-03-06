#include "AssetManagerSerializer.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <unordered_map>
#include<Core/DEFINES.h>
#include "API/Texture.h"
/*
	:m_assetManager(assetManager)
{
}

AssetManagerSerializer::~AssetManagerSerializer()
{
}

void AssetManagerSerializer::Serialize(std::string path)
{

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Project" << YAML::Value << "Untitled Project"; // project


	out << YAML::Key << "Assets" << YAML::BeginSeq; //assets


	{
		// out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq; //tex
		// out << YAML::Key << "Textures" << YAML::BeginSeq; //textures
		for (const auto& myPair : m_assetManager->m_Savemap) {
			out << YAML::BeginMap; // y
			out << YAML::Key << myPair.first << YAML::Value << myPair.second;
			out << YAML::EndMap; // y
		}
		for (const auto& myPair : m_assetManager->m_Savemap) {
			out << YAML::BeginMap; // y
			out << YAML::Key << myPair.first << YAML::Value << myPair.second;
			out << YAML::EndMap; // y
		}
		for (const auto& myPair : m_assetManager->m_Savemap) {
			out << YAML::BeginMap; // y
			out << YAML::Key << myPair.first << YAML::Value << myPair.second;
			out << YAML::EndMap; // y
		}
		// out << YAML::EndSeq; //textures
		
		// out << YAML::EndSeq; //tex
	}


	out << YAML::EndSeq; //assets
	out << YAML::EndMap; //project

	std::ofstream fout(path);
	fout << out.c_str();
}

void AssetManagerSerializer::DeSerialize(std::string path)
{

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

	if (!data["Project"])
		return;

	std::string ProjectName = data["Project"].as<std::string>();

	auto assets = data["Assets"];
	int ID = 0;
	for (auto texture : assets)
	{
		auto Texpath = texture[std::to_string(ID)].as<std::string>();
		m_assetManager->m_Savemap.insert({ ID, Texpath});
		m_assetManager->m_Runmtimemap.insert({ ID, MakeRef<Texture>(Texpath.c_str()) });
		ID++;
	}

	m_assetManager->counter = ID;
}
*/