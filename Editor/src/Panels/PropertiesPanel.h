#pragma once
#include "Panel.h"
#include "Core/DEFINES.h"
#include "ViewportPanel.h"

class PropertiesPanel : public Panel
{
public:
	PropertiesPanel(Ref<SceneHierarchyPanel>& SHP, Ref<ViewportPanel> viewport, Ref<GameView>& gameView);

	~PropertiesPanel();

	virtual void Draw() override;

	void DrawComponents(Entity e);
	void AddComponentButton();

private:
	Ref<SceneHierarchyPanel>& m_sceneHierarchyPanel;
	Ref<ViewportPanel> m_viewport;
	Ref<GameView>& m_gameView;

};
