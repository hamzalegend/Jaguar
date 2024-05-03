#pragma once

#include "Layer/Layer.h"

#include "Scene/Scene.h"

#include <vector>

#include <Serialization/SceneSerializer.h>

#include <API/FrameBuffer.h>

#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/PropertiesPanel.h"

class EditorLayer : public Layer
{
public:
	EditorLayer();
	~EditorLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnUIUpdate();

	void OnScenePlay();
	void OnSceneStop();

	void OpenScene(std::filesystem::path path);
	void SaveScene(std::filesystem::path path);

private:

	void showDefaultUI();

	// Essential
	Ref<Scene> m_scene = nullptr;
	Ref<Scene> m_EditorScene = nullptr;
	Ref<AssetManager> m_assetManager = MakeRef<AssetManager>();
	Ref<SceneSerializer> serializer = nullptr;

	// Panels
	Ref<ContentBrowserPanel> m_ContentBrowserPanel;
	Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
	Ref<PropertiesPanel> m_PropertiesPanel;
	Ref<ViewportPanel> m_ViewportPanel;
	Ref<GameView> m_GameViewPanel;

	// other
	glm::vec2 m_ViewportBounds[2];
	Ref<EditorCameraController> m_EditorCameraController;

	// FrameBuffers
	Ref<FrameBuffer> m_ViewPortFrameBuffer;
	Ref<FrameBuffer> m_GameViewFrameBuffer;

	bool m_inPlayMode = false;

	friend class ViewportPanel;
};