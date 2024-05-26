#include "SceneHierarchyPanel.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/euler_angles.hpp"

SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene>& scene)
	:m_scene(scene)
{
}

SceneHierarchyPanel::~SceneHierarchyPanel()
{
}

void SceneHierarchyPanel::Draw()
{
	ImGui::Begin("Scene Haierachy");

	if (ImGui::Button("+", { 20, 20 }))
		m_selectedEntity = m_scene->CreateEntity();

	auto group = m_scene->m_Registry.group<TagComponent>(entt::get<TransformComponent>);
	for (auto& entityID : group)
	{
	}
		Entity e = m_scene->GetEntityByUUID(0);
		if (e != -1)
			DrawEntityNode(e);

	if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
		m_selectedEntity = { (entt::entity)-1, m_scene.get() };

	if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
			m_scene->CreateEntity();
		ImGui::EndPopup();
	}

	ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity e)
{
	std::string& name = e.GetComponent<TagComponent>().name;


	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_selectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0);

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, name.c_str());
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("ID", (const void*)&e, sizeof(int));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ID");
		if (payload)
		{
			int* v = (int*)payload->Data;
			Entity ent((entt::entity)*v, m_scene.get());
			auto& PTC = e.GetComponent<TransformComponent>();
			auto& CTC = ent.GetComponent<TransformComponent>();

			glm::vec3 position, scale;
			glm::quat rotation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(glm::inverse(PTC.GetGlobalTransform()) * CTC.GetGlobalTransform(), scale, rotation, position, skew, perspective);

			CTC.Position = position;
			CTC.Rotation = glm::eulerAngles(rotation);
			CTC.Scale = scale;

			ent.SetParent(e.GetComponent<UUIDComponent>().uuid);
		}
		else
		{
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemClicked())
	{
		m_selectedEntity = e;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
	{
		if (ImGui::MenuItem("Destroy Entity"))
			entityDeleted = true;
		ImGui::EndPopup();
	}

	if (opened)
	{
		for (auto [uuid, _] : e.GetComponent<TransformComponent>().Children)
			DrawEntityNode(m_scene->GetEntityByUUID(uuid));

		ImGui::TreePop();
	}
	if (entityDeleted)
	{
		m_scene->DestroyEntity(e);
		m_selectedEntity = { (entt::entity)-1, m_scene.get() };
	}
}
