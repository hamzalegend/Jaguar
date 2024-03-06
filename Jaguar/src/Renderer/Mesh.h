#pragma once
#include <vector>

#include "../API/Shader.h"
#include "../API/Texture.h"
#include "../API/Buffer.h"
#include <string>

#include "Material.h"

struct Sprite
{
	glm::vec4 color = {255, 255, 255, 255};
	Ref<Texture> texture;
	std::string texturePath;

	int ID = -1; // Editor Only!! EntityID
};

struct Mesh {
	std::vector<float>* vertices = new std::vector<float>;
	std::vector<unsigned int>* indices = new std::vector<unsigned int>;
	std::string meshPath;
	std::string texturePath;
	// Ref<Texture> texture;
	Ref<Material> Material;

	int ID = -1; // Editor Only!! EntityID
};
