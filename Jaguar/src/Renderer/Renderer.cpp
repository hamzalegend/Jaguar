#include "Renderer.h"

// #include "../Camera/cameraController.h"
#include <iostream>

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Core/Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "API/Buffer.h"
#include "API/Shader.h"
#include "API/Texture.h"
#include "API/Shader.h"

#include "Mesh.h"

#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Camera/Camera.h"


#include <string>



static std::vector<MeshRendererComponent> m_Meshes;
static std::vector<glm::mat4> m_transforms;
static std::vector<lightComponent> m_lights;

static std::vector<glm::vec3> m_lightTransforms;


static std::vector<Sprite> m_sprites;
static std::vector<glm::mat4> m_transforms2D;
static Application* m_app;

// static Ref<Texture> m_texture1;
static Ref<Shader> m_shader;
static Ref<Shader> m_qshader;

Ref<VertexBuffer> qvb;
Ref<VertexArray> qva;
Ref<IndexBuffer> qib;


static std::vector<float> QuadVertices = {
	// positions				// texture coords
	0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left     
};
static std::vector<unsigned int> QuadIndices = {
	0, 1, 3,  // first Triangle
	1, 2, 3   // second Triangle
};



void Renderer::Init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << ("[ERROR]: glad init error!");
		return;
	}
	std::cout << "[INIT RENDERER]::OpenGL\n";
	glClearColor(1, 1, 0, 1);
	glEnable(GL_DEPTH_TEST);

	m_app = &Application::Get();

	m_shader = MakeRef<Shader>("Assets/Shaders/basic.vs", "Assets/Shaders/basic.fs");
	// m_texture1 = MakeRef<Texture>("Assets/Textures/backpack.jpg", m_shader);
	m_qshader = MakeRef<Shader>("Assets/Shaders/sprite.vs", "Assets/Shaders/sprite.fs");

	qvb = MakeRef<VertexBuffer>(QuadVertices);
	qva = MakeRef<VertexArray>();
	qib = MakeRef<IndexBuffer>(QuadIndices);

	qva->Bind();
	qva->Addlayout(3); // Positions
	qva->Addlayout(2); // TexCoords
	// qva->Addlayout(1); // Editor Only!! EntityID
	qva->Flush();
}

void Renderer::SubmitMesh(MeshRendererComponent& mesh, glm::mat4 Transform)
{
	m_Meshes.push_back(mesh);
	m_transforms.push_back(Transform);
}


void Renderer::SubmitSprite(glm::mat4 Transform, Sprite sprite)
{
	m_sprites.push_back(sprite);
	m_transforms2D.push_back(Transform);
}

void Renderer::Flush(glm::mat4 projection, glm::mat4 view)
{

	for (int i = 0; i < m_Meshes.size(); i++)
	{
		Mesh& mesh = m_Meshes[i].mesh;
		auto& MRC = m_Meshes[i];

		MRC.vb->Bind();
		MRC.va->Bind();
		MRC.ib->Bind();

		Ref<Material> material = mesh.Material;

		if (!material) continue;

		material->Shader->Bind();

		if (material->Albedo)
		{
			material->Albedo->Bind();
			material->Shader->SetInt("texture1", 0);
		}

		material->Shader->SetMat4("model", m_transforms[i]);
		material->Shader->SetMat4("view", view);
		material->Shader->SetMat4("projection", projection);

		material->Shader->SetInt("ID", mesh.ID);

		// lighting
		if (m_lights.size())
		{
			int nplights = 0;
			int nslights = 0;
			for (int i = 0; i < m_lights.size(); i++)
			{
				lightComponent light = m_lights[i];


				if (light.Type == lightComponent::LightType::Directional)
				{
					material->Shader->SetVec3("dlight.Direction", m_lightTransforms[2 * i + 1]);
					material->Shader->SetVec4("dlight.Color", light.Color);
					material->Shader->SetFloat("dlight.Intensity", light.Intensity);
				}
				else if (light.Type == lightComponent::LightType::Point)
				{
					std::string LightName = ("plights[" + std::to_string(nplights) + "]");

					material->Shader->SetVec3((LightName + ".Position").c_str(), m_lightTransforms[2 * i]);
					material->Shader->SetVec4((LightName + ".Color").c_str(), light.Color);
					material->Shader->SetFloat((LightName + ".Intensity").c_str(), light.Intensity);
					material->Shader->SetFloat((LightName + ".Linear").c_str(), light.linear);
					
					nplights++;
				}
				else if (light.Type == lightComponent::LightType::Spot)
				{
					
					std::string LightName = ("slights[" + std::to_string(nslights) + "]");

					material->Shader->SetVec3((LightName + ".Position").c_str(), m_lightTransforms[2 * i]);
					material->Shader->SetVec3((LightName + ".Direction").c_str(), m_lightTransforms[2 * i+1]);
					material->Shader->SetVec4((LightName + ".Color").c_str(), light.Color);
					material->Shader->SetFloat((LightName + ".Intensity").c_str(), light.Intensity);
					material->Shader->SetFloat((LightName + ".Linear").c_str(), light.linear);

					material->Shader->SetFloat((LightName + ".CutOff").c_str(), glm::cos(glm::radians(light.CutOff)));
					material->Shader->SetFloat((LightName + ".OuterCutOff").c_str(), glm::cos(glm::radians(light.OuterCutOff)));


					nslights++;
				}
			}
		}

		glDrawElements(GL_TRIANGLES, mesh.indices->size(), GL_UNSIGNED_INT, 0);
		if (material->Albedo)
		{
			material->Albedo->UnBind();
		}

		material->Shader->UnBind();
	}
	m_Meshes.clear();
	m_transforms.clear();


	for (int i = 0; i < m_sprites.size(); i++)
	{
		qvb->Bind();
		qva->Bind();
		qib->Bind();

		m_qshader->Bind();
		// m_texture1->Bind(0);
		if (m_sprites[i].texture)
		{
			m_sprites[i].texture->Bind();
			m_qshader->SetInt("texture1", 0);
		}
		m_qshader->SetVec4("color", glm::vec4(m_sprites[i].color));

		m_qshader->SetMat4("model", m_transforms2D[i]);
		m_qshader->SetMat4("view", view);
		// m_qshader->SetMat4("view", Application::Get().getCamera()->GetProjectionMatrix());
		m_qshader->SetMat4("projection", projection);

		m_qshader->SetInt("ID", m_sprites[i].ID); // temp
		glDrawElements(GL_TRIANGLES, QuadIndices.size(), GL_UNSIGNED_INT, 0);

		if (m_sprites[i].texture)
		{
			m_sprites[i].texture->UnBind();
		}
	}
	m_sprites.clear();
	m_transforms2D.clear();
	m_lights.clear();
	m_lightTransforms.clear();

}

void Renderer::RednerScene(Scene* scene, Camera* cam, glm::mat4 viewMat)
{
	{
		auto group = scene->m_Registry.group<MeshRendererComponent>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			TransformComponent& transform = group.get<TransformComponent>(entity);
			MeshRendererComponent& MRC = group.get<MeshRendererComponent>(entity);
			MRC.mesh.ID = (int)entity;
			if (MRC.mesh.vertices->size() > 0)
				Renderer::SubmitMesh(MRC, transform.GetGlobalTransform());
		}
	}
	{
		auto group = scene->m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			const TransformComponent& transform = group.get<TransformComponent>(entity);
			SpriteRendererComponent& spriteRenderer = group.get<SpriteRendererComponent>(entity);
			spriteRenderer.sprite.ID = (int)entity;
			Renderer::SubmitSprite(transform.GetGlobalTransform(), spriteRenderer.sprite);
		}
	}
	{
		auto group = scene->m_Registry.group<lightComponent>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			TransformComponent& transform = group.get<TransformComponent>(entity);
			lightComponent& LC = group.get<lightComponent>(entity);
			m_lights.push_back(LC);
			m_lightTransforms.push_back(glm::vec3(transform.Position));
			m_lightTransforms.push_back(glm::vec3(transform.Rotation));
		}
	}

	Renderer::Flush(cam->GetProjection(), viewMat);
}