#pragma once
#include "Panel.h"
#include "Core/DEFINES.h"
#include "ViewportPanel.h"

class PropertiesPanel : public Panel
{
public:
	PropertiesPanel(Ref<SceneHierarchyPanel>& SHP, Ref<ViewportPanel> viewport, Ref<GameView>& gameView, bool* inPlayMode, Ref<Scene>& scene);

	~PropertiesPanel();

	virtual void Draw() override;
	inline bool InPlayMode() { return *m_inplayMode; }

	void DrawComponents(Entity e);
	void AddComponentButton();

private:
	Ref<SceneHierarchyPanel>& m_sceneHierarchyPanel;
	Ref<ViewportPanel> m_viewport;
	Ref<GameView>& m_gameView;
	Ref<Scene>& m_Scene;
	bool* m_inplayMode;

};
