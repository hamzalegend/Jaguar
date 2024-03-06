#include "Window.h"

Window::Window(std::string name, glm::vec2 dimentions)
	:m_name(name), m_dimentions(dimentions)
{
	if (!glfwInit())
		return;

	m_window = glfwCreateWindow(m_dimentions.x, m_dimentions.y, m_name.c_str(), NULL, NULL);

	if (!m_window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

Window::~Window()
{
}
