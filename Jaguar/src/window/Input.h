#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Core/Application.h"
#include <iostream>

static glm::vec2* Offset;
static bool m_locked;


static float m_lastx2;
static float m_lasty2;

static float Xoffset2;
static float Yoffset2;


static double XposFirst = 0;
static double YposFirst = 0;

static bool lastframecammove;

static bool m_Enabled;

class Input
{
public:
	static void Init()
	{
		// glfwSetCursorPosCallback(Application::Get().GetWindow()->GetNative(), mouse_callback);
		Offset = new glm::vec2();
		Enable();
	}

	static bool GetKey(int key)
	{ 
		// if (!m_Enabled) return false;
		return glfwGetKey((GLFWwindow*)Application::Get().GetWindow()->GetNative(), key);
	};

	static void SetShowCursor(bool Show)
	{
		if (!Show) glfwSetInputMode(Application::Get().GetWindow()->GetNative(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(Application::Get().GetWindow()->GetNative(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	static void SetLockCursor(bool Locked)
	{
		m_locked = Locked;
	}

	static void Update()
	{
		// double xposIn, yposIn;
		// glfwGetCursorPos(Application::Get().GetWindow()->GetNative(), &xposIn, &yposIn);
		// 
		// float xpos = static_cast<float>(xposIn);
		// float ypos = static_cast<float>(yposIn);
		// 
		// Offset->x = xpos - m_lastx;
		// Offset->y = m_lasty - ypos; // reversed since y-coordinates go from bottom to top
		// 
		// m_lastx = xpos;
		// m_lasty = ypos;

		// std::cout << Offset->x << "\n";
		// std::cout << getDeltaMouse()->x << "\n";

	}

	static glm::vec2 getDeltaMouse()
	{
		if (m_locked) // if u want to lock mouse to screeen middle
		{
			double xposIn, yposIn;
			glfwGetCursorPos(Application::Get().GetWindow()->GetNative(), &xposIn, &yposIn);

			float xpos = static_cast<float>(xposIn);
			float ypos = static_cast<float>(yposIn);

			Xoffset2 = xpos - m_lastx2;
			Yoffset2 = m_lasty2 - ypos; // reversed since y-coordinates go from bottom to top

			m_lastx2 = xpos;
			m_lasty2 = ypos;

		}
		else
		{
			// Input::SetCursorLock(true);
			int x, y;
			double xposIn, yposIn;
			glfwGetWindowSize(Application::Get().GetWindow()->GetNative(), &x, &y);
			glfwGetCursorPos(Application::Get().GetWindow()->GetNative(), &xposIn, &yposIn);

			Xoffset2 = xposIn - XposFirst;
			Yoffset2 = YposFirst - yposIn;


			glfwSetCursorPos(Application::Get().GetWindow()->GetNative(), XposFirst, YposFirst);

		}
		return { Xoffset2, Yoffset2 };
	}


	inline static void Enable() { m_Enabled = true; }
	inline static void Disable() { m_Enabled = false; }

private:

};