#pragma once

#include "Panel.h"

class SceneHierarchyPanel : public Panel
{
public:
	SceneHierarchyPanel(Ref<Scene>& scene);
	~SceneHierarchyPanel();

	virtual void Draw() override;
	void DrawEntityNode(Entity);

	inline Entity GetSeletedEntity() { return m_selectedEntity; };
	inline void ClearSeletedEntity() { m_selectedEntity = { (entt::entity)-1, m_scene.get() }; };

private:
	Ref<Scene>& m_scene;
	Entity m_selectedEntity = { (entt::entity)-1, m_scene.get() };

	friend class EditorLayer;
};

