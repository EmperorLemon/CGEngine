#include "Window.h"

#include "Platform/Windows/GLFWWindow.h"

namespace CGEngine
{
	void CreateWindow(const WindowCreateInfo& windowInfo, Window& window) 
	{
		CreateGLFWWindow(windowInfo, window);
	}

	void DestroyWindow(const Window& window)
	{
		DestroyGLFWWindow(window);
	}

	bool WindowClosed(const Window& window)
	{
		return GLFWWindowClosed(window);
	}

	void PollEvents()
	{
		return PollGLFWEvents();
	}
}
