#include "PropertiesPanel.h"
#include "Scene/Components.h"
#include "SceneHierarchyPanel.h"
#include <filesystem>
#include <imguiLayer/ImguiLayer.h>

#include "Renderer/Material.h"
#include <unordered_map>

#include "Scripting/ScriptEngine.h"
#include "Core/UUID.h"


PropertiesPanel::PropertiesPanel(Ref<SceneHierarchyPanel>& SHP, Ref<ViewportPanel> viewport, Ref<GameView>& gameView, bool* ipm)
	:m_sceneHierarchyPanel(SHP), m_viewport(viewport), m_gameView(gameView), m_inplayMode(ipm)
{
}

PropertiesPanel::~PropertiesPanel()
{
}

const ImGuiTreeNodeFlags treenodeflags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

template <typename T>
static void DrawComponent(const char* name, Entity e, void (*lambda)(Entity))
{
	if (e.HasComponent<T>())
	{
		bool removedComponent = false;
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treenodeflags, name);
		ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
		if (ImGui::Button("+", ImVec2{ 20, 20 }))
			ImGui::OpenPopup("componentsSettings");

		if (ImGui::BeginPopup("componentsSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
				removedComponent = true;
			ImGui::EndPopup();
		}
		if (open)
		{
			lambda(e);
			ImGui::TreePop();
		}
		if (removedComponent) e.RemoveComponent<T>();
	}
} // TODO::

void PropertiesPanel::Draw()
{
	ImGui::Begin("Properties");
	Entity e = m_sceneHierarchyPanel->GetSeletedEntity();
	if ((int)e != -1)
		DrawComponents(e);

	ImGui::End();
}
static bool IsPlayMode;
void PropertiesPanel::DrawComponents(Entity e)
{	
	IsPlayMode = InPlayMode();

	if (e.HasComponent<TagComponent>())
	{
		if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(), treenodeflags, "Tag Component"))
		{
			ImGui::Text("ID: %i", (int)e);
			auto& tag = e.GetComponent<TagComponent>().name;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("###", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::TreePop();
		}
	}
	if (e.HasComponent<TransformComponent>())
	{
		if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treenodeflags, "Transform Component"))
		{
			auto& transform = e.GetComponent<TransformComponent>();


			glm::vec3 rot = glm::degrees(transform.Rotation);
			ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1);
			ImGui::DragFloat3("Rotation", glm::value_ptr(rot), 0.1);
			ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1);
			transform.Rotation = glm::radians(rot);

			ImGui::TreePop();
		}
	}
	if (e.HasComponent<CameraComponent>())
	{
		bool removedComponent = false;
		bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treenodeflags, "Camera Component");
		ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
		if (ImGui::Button("+", ImVec2{ 20, 20 }))
			ImGui::OpenPopup("componentsSettings");

		if (ImGui::BeginPopup("componentsSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
				removedComponent = true;
			ImGui::EndPopup();
		}
		if (open)
		{
			auto& CC = e.GetComponent<CameraComponent>();
			bool oldprimary = CC.Primary;
			ImGui::Checkbox("primary", &CC.Primary);
			if (ImGui::DragFloat("FOV", &CC.camera.FOV) ||
				ImGui::DragFloat("NearPlane", &CC.camera.NearPlane) ||
				ImGui::DragFloat("FarPlane", &CC.camera.FarPlane))

			{
				m_gameView->resizeWindow();
			}

			ImGui::TreePop();
		}
		if (removedComponent) e.RemoveComponent<CameraComponent>();
	}

	DrawComponent<SpriteRendererComponent>("Sprite Renderer Component", e, [](Entity e) {
		auto& SpriteRenderer = e.GetComponent<SpriteRendererComponent>();

		ImGui::ColorEdit4("Color", glm::value_ptr(SpriteRenderer.sprite.color), 1);

		ImGui::Button("Texture");
		std::filesystem::path s_assetsPath = "Assets";
		std::string p = ImguiLayer::RecvDragDrop("CONTENT_BROWSER_ITEM");
		if (p != "") // if you drag in a texture
		{
			std::cout << p << "\n";
			// m_scene = MakeRef<Scene>();
			SpriteRenderer.sprite.texturePath = (s_assetsPath /= p).string();
			SpriteRenderer.sprite.texture = MakeRef<Texture>(SpriteRenderer.sprite.texturePath.c_str());

		}

		ImGui::Text(SpriteRenderer.sprite.texturePath.c_str());
		if (SpriteRenderer.sprite.texture)
			ImGui::Image((ImTextureID)SpriteRenderer.sprite.texture->GetImage(), {56, 56});
		});
	DrawComponent<MeshRendererComponent>("Mesh Renderer Component", e, [](Entity e) {
		auto& MeshRenderer = e.GetComponent<MeshRendererComponent>();

		ImGui::Text("Vertices Count: %i", MeshRenderer.mesh.vertices->size());
		ImGui::Text("Indices Count: %i", MeshRenderer.mesh.indices->size());

		ImGui::Text(("Path: " + MeshRenderer.mesh.meshPath).c_str());
		ImGui::Button("Model");
		std::filesystem::path s_assetsPath = "Assets";
		std::string p = ImguiLayer::RecvDragDrop("CONTENT_BROWSER_ITEM");
		if (p != "") // if you drag in a model
		{
			std::cout << p << "\n";
			MeshRenderer.mesh.meshPath = (s_assetsPath /= p).string();

			ModelManager mm;
			mm.loadModel(MeshRenderer.mesh.meshPath);
			*MeshRenderer.mesh.vertices = mm.getVertexData();
			*MeshRenderer.mesh.indices = mm.getIndexData();
			mm.clear();
			MeshRenderer.ReBuild();

		}

		Ref<Material>& material = MeshRenderer.mesh.Material;
		if (material)
			ImGui::Text(("Path: " + material->Path).c_str());
		ImGui::Button("Material");
		p = ImguiLayer::RecvDragDrop("CONTENT_BROWSER_ITEM");
		if (p != "") // if you drag in a material
		{
			std::cout << p << std::endl;		
			material = AssetManager::LoadMaterial((s_assetsPath.string() + "/" + p).c_str());
		}
		});
	DrawComponent<NativeScriptComponent>("Native Script Component", e, [](Entity e) {
		auto& NSC = e.GetComponent<NativeScriptComponent>();

		ImGui::Text("Script: ");

		});
	DrawComponent<RigidBody2DComponent>("Rigid Body 2D Component", e, [](Entity e) {
		auto& RBC = e.GetComponent<RigidBody2DComponent>();

		// dropdown
		const char* BodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
		const char* CurrentBodyTypeString = BodyTypeStrings[(int)RBC.Type];
		if (ImGui::BeginCombo("Body Type", CurrentBodyTypeString))
		{
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = CurrentBodyTypeString == BodyTypeStrings[i];

				if (ImGui::Selectable(BodyTypeStrings[i], isSelected))
				{
					CurrentBodyTypeString = BodyTypeStrings[i];
					RBC.Type = (RigidBody2DComponent::BodyType)i;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Checkbox("fixedRotation", &RBC.FixedRotation);

		});
	DrawComponent<BoxCollider2DComponent>("Box Collider 2D Component", e, [](Entity e) {
		auto& BCC = e.GetComponent<BoxCollider2DComponent>();

		// ImGui::Text("BCC");
		ImGui::DragFloat2("Size", glm::value_ptr(BCC.size));
		ImGui::DragFloat2("Offset", glm::value_ptr(BCC.offset));
		ImGui::DragFloat("Density", &BCC.Density);
		ImGui::DragFloat("Friction", &BCC.Friction);
		ImGui::DragFloat("Restitution", &BCC.Restitution);
		ImGui::DragFloat("RestitutionThreshhold", &BCC.RestitutionThreshhold);
		});

	DrawComponent<lightComponent>("light Component", e, [](Entity e) {
		auto& LC = e.GetComponent<lightComponent>();

		// dropdown
		const char* LightTypeStrings[] = { "Directional", "Point", "Spot" };
		const char* CurrentLightTypeString = LightTypeStrings[(int)LC.Type];
		if (ImGui::BeginCombo("Light Type", CurrentLightTypeString))
		{
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = CurrentLightTypeString == LightTypeStrings[i];

				if (ImGui::Selectable(LightTypeStrings[i], isSelected))
				{
					CurrentLightTypeString = LightTypeStrings[i];
					LC.Type = (lightComponent::LightType)i;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::ColorEdit4("Color", (float*)& LC.Color);
		ImGui::DragFloat("Intensity", &LC.Intensity, 0.01f);

		if (LC.Type == lightComponent::LightType::Point)
		{
			ImGui::DragFloat("linear", &LC.linear, 0.01f, 0);
			ImGui::DragFloat("c", &LC.constant, 0.01f, 0);
			ImGui::DragFloat("qa", &LC.quadratic, 0.1f, 0);
		}
		if (LC.Type == lightComponent::LightType::Spot)
		{
			
			ImGui::DragFloat("linear", &LC.linear, 0.01f, 0);
			ImGui::DragFloat("c", &LC.constant, 0.01f, 0);
			ImGui::DragFloat("qa", &LC.quadratic, 0.1f, 0);

			ImGui::DragFloat("CutOff", &LC.CutOff, 1);
			ImGui::DragFloat("OuterCutOff", &LC.OuterCutOff, 1);
		}
		});
	DrawComponent<ScriptComponent>("Script Component", e, [](Entity e) {
		auto& SC = e.GetComponent<ScriptComponent>();

		bool scriptClassExists = ScriptEngine::EntityClassExists(SC.ClassName);

		static char buffer[64];
		strcpy(buffer, SC.ClassName.c_str());

		if (!scriptClassExists)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

		if (ImGui::InputText("Class", buffer, sizeof(buffer)))
			SC.ClassName = buffer;

		const Jaguar::UUID uuid = e.GetComponent<UUIDComponent>().uuid;
		Ref<ScriptInstance> SI = ScriptEngine::GetEntityScriptInstance(uuid);

		if (scriptClassExists)
		{
			auto EntityClass = ScriptEngine::GetEntityClasses().find(SC.ClassName)->second;
			for (const auto& [name, field] : EntityClass->GetFields())
			{
				if (field.type == ScriptFieldType::Float)
				{
					auto& instance = ScriptEngine::GetScriptFieldInstance(name, uuid);
					float data = 0;
					if (IsPlayMode)	
						data = SI->GetFieldValue<float>(name);
					else data = instance.GetValue<float>();
					
					if (ImGui::DragFloat(name.c_str(), &data))
					{
						if (IsPlayMode)
						{
							if (SI) SI->SetFieldValue(name, data);
						}
						else
						{
							instance.SetFieldValue(data);
							ScriptEngine::SetScriptFieldInstance(name, uuid, instance);
						}
					}
				}

			}
		}

		if (!scriptClassExists)
			ImGui::PopStyleColor();

		});

	if (m_sceneHierarchyPanel->GetSeletedEntity() != -1)
		AddComponentButton();

}

void PropertiesPanel::AddComponentButton()
{
	static int selectedComponent = -1;
	const char* names[] = { "Tag Component", "Transform Component", "Sprite Renderer Component", "Mesh Renderer Component", "Camera Component", "Rigid Body2D Component", "BoxCollider2D Component", "LightComponent", "Script Component"};

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponentPopup");

	if (ImGui::BeginPopup("AddComponentPopup"))
	{
		ImGui::SeparatorText("Add:");
		for (int i = 0; i < IM_ARRAYSIZE(names); i++)
			if (ImGui::Selectable(names[i]))
			{

				switch (i)
				{
				case 0: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<TagComponent>(); break;
				case 1: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<TransformComponent>(); break;
				case 2: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<SpriteRendererComponent>(); break;
				case 3: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<MeshRendererComponent>(); break;
				case 4: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<CameraComponent>(); break;
				case 5: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<RigidBody2DComponent>(); break;
				case 6: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<BoxCollider2DComponent>(); break;
				case 7: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<lightComponent>(); break;
				case 8: m_sceneHierarchyPanel->GetSeletedEntity().AddComponent<ScriptComponent>(); break;
				}
				selectedComponent = -1;
				ImGui::CloseCurrentPopup();
			}
		ImGui::EndPopup();
	}
}
