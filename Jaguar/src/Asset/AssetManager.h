#pragma once
#include "API/Texture.h"

#include <unordered_map>

#include <string>
#include <iostream>

#include "Asset.h"

#include "core/DEFINES.h"
#include "AssetManager.h"
#include "Renderer/Material.h"
#include <filesystem>

// #include <any>

class JAGUAR_API AssetManager
{
public:
	AssetManager();
	~AssetManager();

	// int LoadTexture(std::string path);
	// Ref<Texture> GetTexture(int ID);

	// template <typename T>
	// int Load(std::string path);
	// template <typename T>
	// Ref<T> Get(int ID);

	static Ref<Material> LoadMaterial(const char* path);
	static void SaveMaterial(std::filesystem::path path, Ref<Material>& material);

private:
	// std::unordered_map<int, std::string> m_Savemap;
	// std::unordered_map<int, Ref<std::any>> m_Runmtimemap;

	int counter = -1;
	inline int GenID() { counter++; return counter; };

	friend class AssetManagerSerializer;

};
// AssetManager assetManager;
// int id = assetManager.LoadShader(path);
// Ref<Shader> shader assetManager.Get<Ref<Shader>>(id);