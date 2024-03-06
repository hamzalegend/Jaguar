#include "SandBoxLayer.h"
#include <iostream>

#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include "Renderer/ModelLoader.h"
#include <window/Input.h>
#include "imguilayer/ImguiLayer.h"

#include "Scene/Entity.h"
#include "Scene/Components.h"

// #include "imgui/imgui_demo.cpp"

#include "glad/glad.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif


SandBoxLayer::SandBoxLayer()
{
}

SandBoxLayer::~SandBoxLayer()
{
}

static Ref<FrameBuffer> m_frameBuffer = nullptr;
static int layloo = 0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Application::Get().GetWindow()->SetDimentions({ width, height });

}


void SandBoxLayer::OnAttach()
{
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetFramebufferSizeCallback(Application::Get().GetWindow()->GetNative(), framebuffer_size_callback);
	framebuffer_size_callback(Application::Get().GetWindow()->GetNative(), Application::Get().GetWindow()->GetWindowDimentions().x, Application::Get().GetWindow()->GetWindowDimentions().y);

	Input::SetShowCursor(true);

	m_scene = MakeRef<Scene>();
	serializer = MakeRef<SceneSerializer>(m_scene, m_assetManager);
	
	FrameBufferSpecification FBspec = { 1280, 720 };
	FBspec.Atachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::Depth };
	m_frameBuffer = MakeRef<FrameBuffer>(FBspec);

	serializer->DeSerialize("Assets/Scenes/2D.Hscn");	
	m_scene->OnPhysicsStart();
	ImguiLayer::init(Application::Get().GetWindow()->GetNative());
};

void SandBoxLayer::OnUpdate(float DeltaTime)
{
	// ImguiLayer::Begin();
	if (Input::GetKey(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(Application::Get().GetWindow()->GetNative(), true);

	// m_frameBuffer->Bind();
	m_scene->OnUpdate(DeltaTime, true);
	m_frameBuffer->UnBind();

	if (Input::GetKey(GLFW_KEY_SPACE)) std::cout << "space!!\n";
	// ImguiLayer::End();
    OnUIUpdate();
}

void SandBoxLayer::OnUIUpdate()
{
    // mguiLayer::init(Application::Get().GetWindow()->GetNative());
    ImguiLayer::Begin();
    {
        ImGui::Begin("Hello");
        ImGui::Text("Hello sandbox");
        ImGui::End();
    }
    ImguiLayer::End();
}

void SandBoxLayer::OnDetach()
{
	// ImguiLayer::Destroy();
}
/*
// Main code
int main(int, char**)
{
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
*/