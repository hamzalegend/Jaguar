#pragma once

#include "Core/DEFINES.h"
#include "window/Window.h"

#include "Layer/Layer.h"

#include <vector>

class JAGUAR_API Application
{
public:
	Application();
	~Application();

	void Run();

	inline void PushLayer(Ref<Layer> layer) { m_LayerStack.push_back(layer); }

	inline static Application& Get() { return *s_Instance; }

	inline Window* GetWindow() { return m_window; }

private:

	std::vector<Ref<Layer>> m_LayerStack;

	Window* m_window = nullptr;

	static Application* s_Instance;

};
