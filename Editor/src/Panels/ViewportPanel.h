#pragma once

#include "Panel.h"
#include "Serialization/SceneSerializer.h"
#include "Core/DEFINES.h"

class EditorLayer;

class ViewportPanel : public Panel
{
public:
	ViewportPanel(EditorLayer* editor);
	~ViewportPanel();

	virtual void Draw() override;
	void resizeWindow();

	bool MoveCam;
private:
	EditorLayer* m_editor;

	glm::vec2 m_ViewportSize = { 0, 0 };
	ImGuizmo::OPERATION EditorOperation;
};


class GameView : public Panel
{
public:
	GameView(Ref<FrameBuffer>& frameBuffer, Ref<Scene>& scene);
	~GameView();

	virtual void Draw() override;
	void resizeWindow();

private:
	Ref<Scene>& m_scene;
	Ref<FrameBuffer> m_frameBuffer;
	glm::vec2 m_ViewportSize = { 0, 0 };

};