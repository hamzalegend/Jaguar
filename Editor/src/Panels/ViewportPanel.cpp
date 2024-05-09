#include "ViewportPanel.h"
#include "Core/DEFINES.h"
#include "math/math.h"
#include "SceneHierarchyPanel.h"

#include "../Editor.h"

#include <filesystem>
#include "Engine.h"

static bool ourDecomposeTransform(const glm::mat4 transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
{

	// From glm::decompose in matrix_decompose.inl

	using namespace glm;
	using T = float;

	mat4 LocalMatrix(transform);

	// Normalize the matrix.
	if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
		return false;

	// First, isolate perspective.  This is the messiest.
	if (
		epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
		epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
		epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
	{
		// Clear the perspective partition
		LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
		LocalMatrix[3][3] = static_cast<T>(1);
	}

	// Next take care of translation (easy).
	translation = vec3(LocalMatrix[3]);
	LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

	vec3 Row[3], Pdum3;

	// Now get scale and shear.
	for (length_t i = 0; i < 3; ++i)
		for (length_t j = 0; j < 3; ++j)
			Row[i][j] = LocalMatrix[i][j];

	// Compute X scale factor and normalize first row.
	scale.x = length(Row[0]);
	Row[0] = detail::scale(Row[0], static_cast<T>(1));
	scale.y = length(Row[1]);
	Row[1] = detail::scale(Row[1], static_cast<T>(1));
	scale.z = length(Row[2]);
	Row[2] = detail::scale(Row[2], static_cast<T>(1));

	rotation.y = asin(-Row[0][2]);
	if (cos(rotation.y) != 0) {
		rotation.x = atan2(Row[1][2], Row[2][2]);
		rotation.z = atan2(Row[0][1], Row[0][0]);
	}
	else {
		rotation.x = atan2(-Row[2][0], Row[1][1]);
		rotation.z = 0;
	}


	return true;
};
ViewportPanel::ViewportPanel(EditorLayer* editor)
:m_editor(editor), EditorOperation(ImGuizmo::OPERATION::TRANSLATE), MoveCam(false)
{
}

ViewportPanel::~ViewportPanel()
{
}

void ViewportPanel::Draw()
{

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");

	// camera Movement:
	{
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(1))
		{
			Input::SetLockCursor(true);
			Input::SetShowCursor(false);
			MoveCam = true;
		}
		else
		{
			MoveCam = false;
			Input::SetLockCursor(false);
			Input::SetShowCursor(true);

			if (Input::GetKey(GLFW_KEY_W))
				EditorOperation = ImGuizmo::OPERATION::TRANSLATE;
			if (Input::GetKey(GLFW_KEY_E))
				EditorOperation = ImGuizmo::OPERATION::ROTATE;
			if (Input::GetKey(GLFW_KEY_R))
				EditorOperation = ImGuizmo::OPERATION::SCALE;
		}
	}

	// Draw
	{
		ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != glm::vec2(ViewportPanelSize.x, ViewportPanelSize.y))
		{
			m_ViewportSize = {ViewportPanelSize.x, ViewportPanelSize.y};
			resizeWindow();
		}
		ImGui::Image((void *)m_editor->m_ViewPortFrameBuffer->getColorAttachmentID(0), {ViewportPanelSize.x, ViewportPanelSize.y});
		// ImGui::Image((void*)m_frameBuffer->getDepthAttachmentID(), { ViewportPanelSize.x, ViewportPanelSize.y });
	}

	// Mouse Picking
	{
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		// viewportOffset;
		m_editor->m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_editor->m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
	}

	// loadin scenes
	if (ImGui::BeginDragDropTarget())
	{
		std::filesystem::path s_assetsPath = "Assets";
		const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
		if (payload)
		{
			const wchar_t *path = (wchar_t *)payload->Data;

			std::wstring ws(path);
			std::string p(ws.begin(), ws.end());

			m_editor->OpenScene((s_assetsPath /= p).string());
			m_editor->m_SceneHierarchyPanel->ClearSeletedEntity();
			m_editor->m_GameViewPanel->resizeWindow();
		}
		ImGui::EndDragDropTarget();
	}

	// Gizmos:
	Entity e = m_editor->m_SceneHierarchyPanel->GetSeletedEntity();
	if (e != -1)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float)ImGui::GetWindowWidth(), (float)ImGui::GetWindowHeight());

		glm::mat4 proj = m_editor->m_EditorCameraController->GetProjection();
		glm::mat4 view = m_editor->m_EditorCameraController->GetViewMatrix();

		auto &tc = e.GetComponent<TransformComponent>();
		glm::mat4 transform = tc.GetTransform();

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(glm::scale(proj, glm::vec3(1, -1, 1))), EditorOperation, ImGuizmo::LOCAL, glm::value_ptr(transform));

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			ourDecomposeTransform(transform, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - tc.Rotation;
			tc.Position = translation;
			tc.Rotation += deltaRotation;
			tc.Scale = scale;
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void ViewportPanel::resizeWindow()
{
	m_editor->m_ViewPortFrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
	m_editor->m_ViewPortFrameBuffer->Bind();

	m_editor->m_EditorCameraController->UpdateProjectionMatrix(m_ViewportSize.x, m_ViewportSize.y, true);
}

// GameView

GameView::GameView(Ref<FrameBuffer> &frameBuffer, Ref<Scene> &scene)
	: m_frameBuffer(frameBuffer), m_scene(scene)
{
}

GameView::~GameView()
{
}

void GameView::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("GameView");

	ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();
	if (m_ViewportSize != glm::vec2(ViewportPanelSize.x, ViewportPanelSize.y))
	{
		m_ViewportSize = {ViewportPanelSize.x, ViewportPanelSize.y};
		resizeWindow();
	}
	ImGui::Image((void *)m_frameBuffer->getColorAttachmentID(), {ViewportPanelSize.x, ViewportPanelSize.y});
	ImGui::End();
	ImGui::PopStyleVar();
}

void GameView::resizeWindow()
{

	m_frameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
	m_frameBuffer->Bind();

	auto view = m_scene->m_Registry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto& cc = Entity{ entity, m_scene.get() }.GetComponent<CameraComponent>();
		cc.camera.SetProjection(glm::scale(glm::perspective(glm::radians(cc.camera.FOV), m_ViewportSize.x / m_ViewportSize.y, cc.camera.NearPlane, cc.camera.FarPlane), glm::vec3(1, -1, 1)));
	}
	m_frameBuffer->Bind();
}
