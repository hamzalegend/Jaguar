#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"


#include <string>

#include "glm/glm.hpp"

class Window
{
public:
	Window(std::string name, glm::vec2 = {1280, 720});
	~Window();

	inline GLFWwindow* GetNative() { return m_window; }

	glm::vec2 GetWindowDimentions() { return m_dimentions; };

	inline void SetDimentions(glm::vec2 dimentions) { m_dimentions = dimentions; }
private:
	std::string m_name;
	glm::vec2 m_dimentions;

	GLFWwindow* m_window;

};
