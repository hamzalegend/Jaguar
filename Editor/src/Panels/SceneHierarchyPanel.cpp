#include "SceneHierarchyPanel.h"

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
		Entity e = { entityID, m_scene.get() };
		if (e != -1)
			DrawEntityNode(e);
	}

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
	{;
		ImGui::SetDragDropPayload("UUID", (const void*)&e.GetComponent<UUIDComponent>().uuid, sizeof(Jaguar::UUID));
		ImGui::EndDragDropSource();
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
		ImGui::TreePop();
	}
	if (entityDeleted)
	{
		m_scene->DestroyEntity(e);
		m_selectedEntity = { (entt::entity)-1, m_scene.get() };
	}
}
