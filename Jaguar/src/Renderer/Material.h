#pragma once

#include "API/Texture.h"
#include "API/Shader.h"

class Material
{
public:
	// Material();
	// ~Material();
	std::string Path;
	std::string VertexShaderPath;
	std::string FragmentShaderPath;
	std::string AlbedoPath;
	Ref<Texture> Albedo;
	Ref<Shader> Shader;


private:

};