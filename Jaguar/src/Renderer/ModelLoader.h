#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>

#include <Windows.h>
#include <iostream>
#include <stdint.h>
#include <vector>

#include "Core/DEFINES.h"

class JAGUAR_API ModelManager
{
public:
	ModelManager();
	~ModelManager(void);

	bool loadModel(std::string file);
	bool processData();

	std::vector<float> getVertexData();
	std::vector<unsigned int> getIndexData();
	std::vector<std::string> getTextureData();

	void clear();

private:
	bool assimpGetMeshData(const aiMesh* mesh);
private:
	Assimp::Importer            importer;
	const aiScene* modelScene;
	const aiNode* modelNode;
	const aiMesh* modelMesh;
	const aiFace* modelFace;
	std::vector<float>          vertexBuff;
	std::vector<unsigned int>       indexBuff;

	std::vector<const aiNode*>  nodeBuff;
	unsigned int                numNodeBuff;

	std::vector<std::string> m_TexturePaths;
	void processNode(aiNode* node, const aiScene* scene);
};