#include "Application.h"

#include "window/Input.h"
#include "Renderer/Renderer.h"

#include <imguiLayer/ImguiLayer.h>
#include "Scripting/ScriptEngine.h"

Application* Application::s_Instance = nullptr;
Application::Application()
{
	if (s_Instance) std::cout << "[ERROR] 2 Applications!\n";
	s_Instance = this;
}

Application::~Application()
{
	for (Ref<Layer> layer : m_LayerStack)
	{
		layer->OnDetach();
	}
	std::cout << "[Application]::Termenating.\n\n";
}

float deltaTime = 0;
float lastFrame = 0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void Application::Run()
{
	m_window = new Window("Jaguar Engine");

	Input::Init();
	Renderer::Init();
	ScriptEngine::Init();

	for (Ref<Layer> layer : m_LayerStack)
	{
		layer->OnAttach();
	}

	while (!glfwWindowShouldClose(m_window->GetNative()))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Input::Update();
		
		for (Ref<Layer> layer : m_LayerStack)
		{
			layer->OnUpdate(deltaTime);
		}
		
		glfwSwapBuffers(m_window->GetNative());
		glfwPollEvents();

	}

	glfwTerminate();
}
