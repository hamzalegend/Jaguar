#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include<iostream>

#include "../API/Buffer.h"
#include "../API/Shader.h"
#include "../API/Texture.h"
#include "../API/Shader.h"

#include <Core/DEFINES.h>


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

// struct Texture {
//     unsigned int id;
//     std::string type;
// };
// 
struct Mesh_me
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};

class Model
{
public:
	Model(const char* path);
	
	Mesh_me FinalMesh;
	Ref<Shader> shader;
	// Ref<Texture> texture1;

	void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	


	std::vector<Mesh_me> m_meshes;


	Mesh_me processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);


	void loadModel(std::string path);

	
	
};