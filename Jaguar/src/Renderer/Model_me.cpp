#include "Model_me.h"
	
Model::Model(const char* path)
{
	shader = MakeRef<Shader>("Assets/Shaders/basic.vs", "Assets/Shaders/basic.fs");
	loadModel(path);
	// texture1 = MakeRef<Texture>("Assets/cube.png", shader);

}

void Model::Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{

	VertexBuffer vb(FinalMesh.vertices);
	vb.Bind();

	VertexArray va;
	va.Bind();
	va.Addlayout(3); // Positions
	va.Addlayout(3); // Normals
	va.Addlayout(2); // TexCoords
	va.Flush();

	IndexBuffer ib(FinalMesh.indices);
	ib.Bind();


	shader->Bind();
	shader->SetMat4("model", model);
	shader->SetMat4("view", view);
	shader->SetMat4("projection", projection);

	// texture1->Bind(0);

	glDrawElements(GL_TRIANGLES, FinalMesh.indices.size(), GL_UNSIGNED_INT, 0);

	va.UnBind();
	vb.UnBind();
	ib.UnBind();
	shader->UnBind();
	// texture1->UnBind();
}

Mesh_me Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// process vertex positions, normals and texture coordinates

		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		vertices.push_back(mesh->mNormals[i].x);
		vertices.push_back(mesh->mNormals[i].y);
		vertices.push_back(mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{

			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertices.push_back(1);
			vertices.push_back(1);
		}
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	Mesh_me m;
	m.vertices = vertices;
	m.indices = indices;
	return m;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;

	}
	auto directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	for (int i = 0; i < m_meshes.size(); i++)
	{
		Mesh_me mesh = m_meshes[i];

		for (int i = 0; i < mesh.vertices.size(); i++)
		{
			FinalMesh.vertices.push_back(mesh.vertices[i]);
		}

		int offset = FinalMesh.indices.size();
		for (int i = 0; i < mesh.indices.size(); i++)
		{
			FinalMesh.indices.push_back(mesh.indices[i] + offset);
		}
	}

	std::cout << "Loaded model: '" << path << "' with " << FinalMesh.vertices.size() / 8 << " Vertices and " << FinalMesh.indices.size() << " indices.\n";

}





