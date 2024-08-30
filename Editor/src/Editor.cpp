#include "Editor.h"

#include "Editor.h"
#include <iostream>

#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scripting/ScriptEngine.h"


#include <window/Input.h>
#include "imguilayer/ImguiLayer.h"

#include "Scene/Entity.h"
#include "Scene/Components.h"

#include "Renderer/Renderer.h"

#include <windows.h>
#include <string>
#include <iostream>
char buff[64];

EditorLayer::EditorLayer(std::filesystem::path ProjectPath)
:m_ProjectPath(ProjectPath)
{
	*buff = 0;
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnScenePlay()
{
	m_scene->OnRuntimeStart();
	m_EditorScene = Scene::copy(m_scene);
}

void EditorLayer::OnSceneStop()
{
	m_SceneHierarchyPanel->ClearSeletedEntity();
	m_scene->OnRuntimeStop();

	m_scene = Scene::copy(m_EditorScene);
}


void EditorLayer::OpenScene(std::filesystem::path path)
{
	std::cout << path << "\n";
	m_Serializer->DeSerialize(( path).string(), m_ProjectPath);
	ScriptEngine::OnEditorStart(m_scene.get());
}

void EditorLayer::SaveScene(std::filesystem::path path)
{
	m_Serializer->Serialize( path.string());
}


void EditorLayer::OnAttach()
{
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Input::SetShowCursor(true);

	m_scene = MakeRef<Scene>();
	m_Serializer = MakeRef<SceneSerializer>(m_scene, m_assetManager);
	m_scene->CreateEntity("Camera").AddComponent<CameraComponent>();

	{
		FrameBufferSpecification FBspecGameView = { 1280, 720 };
		FBspecGameView.Atachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::Depth };
		m_GameViewFrameBuffer = MakeRef<FrameBuffer>(FBspecGameView);

		FrameBufferSpecification FBspec = { 1280, 720 };
		FBspec.Atachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		m_ViewPortFrameBuffer = MakeRef<FrameBuffer>(FBspec);
	}

	m_EditorCameraController = MakeRef<EditorCameraController>();
	m_ContentBrowserPanel = MakeRef<ContentBrowserPanel>((m_ProjectPath / "Assets").string());
	m_SceneHierarchyPanel = MakeRef<SceneHierarchyPanel>(m_scene);
	m_ViewportPanel = MakeRef<ViewportPanel>(this);
	m_GameViewPanel = MakeRef<GameView>(m_GameViewFrameBuffer, m_scene);
	m_PropertiesPanel = MakeRef<PropertiesPanel>(m_SceneHierarchyPanel, m_ViewportPanel, m_GameViewPanel, &m_inPlayMode, m_scene);

	AssetManager::Init(m_ProjectPath);
	ImguiLayer::init(Application::Get().GetWindow()->GetNative());
	OpenScene("Assets/Scenes/main.jscn");
};

bool lstfrmsim;
void EditorLayer::OnUpdate(float deltaTime)
{

	if (lastframecammove != m_ViewportPanel->MoveCam)
	{
		glfwGetCursorPos(Application::Get().GetWindow()->GetNative(), &XposFirst, &YposFirst);
		lastframecammove = m_ViewportPanel->MoveCam;
	}

	if (Input::GetKey(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(Application::Get().GetWindow()->GetNative(), true);
	if (m_ViewportPanel->MoveCam)
	{
		if (Input::GetKey(GLFW_KEY_W))m_EditorCameraController->ProcessKeyboard(FORWARD, deltaTime);
		if (Input::GetKey(GLFW_KEY_S))m_EditorCameraController->ProcessKeyboard(BACKWARD, deltaTime);

		if (Input::GetKey(GLFW_KEY_D))m_EditorCameraController->ProcessKeyboard(RIGHT, deltaTime);
		if (Input::GetKey(GLFW_KEY_A))m_EditorCameraController->ProcessKeyboard(LEFT, deltaTime);

		if (Input::GetKey(GLFW_KEY_E))m_EditorCameraController->ProcessKeyboard(UP, deltaTime);
		if (Input::GetKey(GLFW_KEY_Q))m_EditorCameraController->ProcessKeyboard(DOWN, deltaTime);

		glm::vec2 offset = Input::getDeltaMouse();
		m_EditorCameraController->ProcessMouseMovement(offset.x, offset.y);

	}
	else
	{
		if (Input::GetKey(GLFW_KEY_F1))
		{
			SaveScene("Scenes/2D.Hscn");
		}
	}

	if (lstfrmsim != m_inPlayMode)
	{
		if (m_inPlayMode)
			OnScenePlay();
		else
			OnSceneStop();
		lstfrmsim = m_inPlayMode;
	}

	m_GameViewFrameBuffer->Bind();
	m_scene->OnUpdate(deltaTime, m_inPlayMode);
	m_GameViewFrameBuffer->UnBind();

	// MousePicking
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		int mouseX = (int)mx;
		int mouseY = (int)my;
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			if (ImGui::IsMouseDown(0) && !ImGuizmo::IsUsing())
			{
				m_ViewPortFrameBuffer->Bind();
				int ID = m_ViewPortFrameBuffer->ReadPixles(1, mouseX, mouseY);
				m_SceneHierarchyPanel->m_selectedEntity = { (entt::entity)ID, m_scene.get() };
				m_ViewPortFrameBuffer->UnBind();
			}
		}
	}

	OnUIUpdate();

}
Ref<Material> mat;
std::string path;

bool ShowSaveWindow = false;



void EditorLayer::OnUIUpdate()
{
	ImguiLayer::Begin();
	showDefaultUI();

	m_ContentBrowserPanel->Draw();
	m_SceneHierarchyPanel->Draw();
	m_PropertiesPanel->Draw();

	m_ViewPortFrameBuffer->Bind();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_ViewPortFrameBuffer->ClearAttachment(1, -1);
	Renderer::RednerScene(m_scene.get(), m_EditorCameraController.get(), m_EditorCameraController->GetViewMatrix());
	m_ViewPortFrameBuffer->UnBind();
	m_ViewportPanel->Draw();

	m_GameViewPanel->Draw();

	{
		ImGui::Begin("Hello");
		ImGui::Text("Hello Editor");
		ImGui::End();
	}

	{
		ImGui::Begin("Material View");

		std::filesystem::path s_assetsPath = "Assets";
		if (!mat)
		{
			ImGui::Button("Materail");
		}
		std::string p = ImguiLayer::RecvDragDrop("CONTENT_BROWSER_ITEM");
		if (p != "") // if you drag in a material
		{
			std::cout << p << std::endl;
			mat = MakeRef<Material>();
			mat = AssetManager::LoadMaterial((s_assetsPath.string() + "/" + p).c_str());
		}

		if (mat)
		{

			ImGui::Image((ImTextureID)mat->Albedo->GetImage(), {56, 56});
			std::string p;


			ImGui::Button("Vertex Shader");
			p = ImguiLayer::RecvDragDrop("CONTENT_BROWSER_ITEM");
			if (p != "")
			{
				mat->VertexShaderPath = "Assets/" + p;
			}
			ImGui::Text(("Vertex Shader Path: " + mat->VertexShaderPath).c_str());


			ImGui::Button("Fragment Shader");
			p = ImguiLayer::RecvDragDrop("CONTENT_BROWSER_ITEM");
			if (p != "")
			{
				mat->FragmentShaderPath = "Assets/" + p;
			}
			ImGui::Text(("Fragment Shader Path: " + mat->FragmentShaderPath).c_str());


			ImGui::Button("Albedo");
			p = ImguiLayer::RecvDragDrop("CONTENT_BROWSER_ITEM");
			if (p != "")
			{
				mat->AlbedoPath = "Assets/" + p;
				mat->Albedo = MakeRef<Texture>(mat->AlbedoPath.c_str());
			}
			ImGui::Text(("Albedo Path: " + mat->AlbedoPath).c_str());


			if (ImGui::Button("Save"))
			{
				AssetManager::SaveMaterial(std::filesystem::path(mat->Path), mat);
			}

		}
		if (ShowSaveWindow)
		{
			ImGui::Begin("Save Scene");
			
			ImGui::InputText("Name: ", buff, sizeof(buff));
			if (ImGui::Button("Save"))
			{
				std::cout << std::string(buff).c_str();
				auto p = (std::filesystem::path(m_ProjectPath) / "Assets/Scenes/") / std::filesystem::path(std::string(buff)+ ".jscn");
				SaveScene(p);
				ShowSaveWindow = false;
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ShowSaveWindow = false;
			}
			ImGui::End();
		}

		ImGui::End();
	}

	{
		ImGui::Begin("ToolBar");
		if (ImGui::Button(!m_inPlayMode ? "Play" : "Stop"))
		{
			m_inPlayMode = !m_inPlayMode;
		}
		ImGui::End();

	}

	{
		ImGui::Begin("Debug Window");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Button("uuid");
		auto& a = ImguiLayer::RecvDragDrop("UUID");
		
		if (ImGui::Button("Debug"))
		{
			logh(a.c_str());
		}

		ImGui::End();

	}



	ImguiLayer::End();
}

void EditorLayer::OnDetach()
{
	ImguiLayer::Destroy();
}

void EditorLayer::showDefaultUI()
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// IMGUI_DEMO_MARKER("Examples/Menu");
			ImGui::MenuItem("(demo menu)", NULL, false, false);
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					if (ImGui::BeginMenu("Recurse.."))
					{
						// ShowExampleMenuFile();
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				ShowSaveWindow = true;
			}
			if (ImGui::MenuItem("Save As..")) {}

			ImGui::Separator();
			// IMGUI_DEMO_MARKER("Examples/Menu/Options");
			if (ImGui::BeginMenu("Options"))
			{
				static bool enabled = true;
				ImGui::MenuItem("Enabled", "", &enabled);
				ImGui::BeginChild("child", ImVec2(0, 60), true);
				for (int i = 0; i < 10; i++)
					ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::EndMenu();
			}

			// IMGUI_DEMO_MARKER("Examples/Menu/Colors");
			if (ImGui::BeginMenu("Colors"))
			{
				float sz = ImGui::GetTextLineHeight();
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
					ImVec2 p = ImGui::GetCursorScreenPos();
					ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
					ImGui::Dummy(ImVec2(sz, sz));
					ImGui::SameLine();
					ImGui::MenuItem(name);
				}
				ImGui::EndMenu();
			}

			// Here we demonstrate appending again to the "Options" menu (which we already created above)
			// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
			// In a real code-base using it would make senses to use this feature from very different code locations.
			if (ImGui::BeginMenu("Options")) // <-- Append!
			{
				// IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
				static bool b = true;
				ImGui::Checkbox("SomeOption", &b);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Disabled", false)) // Disabled
			{
				IM_ASSERT(0);
			}
			if (ImGui::MenuItem("Checked", NULL, true)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Alt+F4")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// for dockspace
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
			window_flags |= ImGuiWindowFlags_NoBackground;
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		//ImGuiID dockspace_id = ImGui::GetMainViewport()->ID;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		ImGui::End();
	}

}
