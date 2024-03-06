#pragma once

#include"Mesh.h"

struct RenderTarget;

#include "../API/Shader.h"
#include "../API/Texture.h"

#include <vector>
#include <iostream>
#include <Core/Application.h>

#include <Scene/Scene.h>
#include <Camera/Camera.h>

#include "Scene/Components.h"

class JAGUAR_API Renderer
{
public:

	static void Init();
	static void SubmitMesh(MeshRendererComponent& mesh, glm::mat4 Transform);
	static void SubmitSprite(glm::mat4 Transform, Sprite sprite);
	static void Flush(glm::mat4 projection, glm::mat4 view);
	
	static void RednerScene(Scene* scene, Camera* cam, glm::mat4 viewMat);

	// static std::vector<Mesh*> m_Meshes;


	// static Shader* m_shader;
	// static Texture* m_texture1;

	// static Shader* m_shader;
private:
	


};
