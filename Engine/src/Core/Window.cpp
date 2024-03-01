#include "Window.h"

#include "Platform/Windows/GLFWWindow.h"

namespace CGEngine
{
	void CreateWindow(const WindowCreateInfo& info, Window& window) 
	{
		CreateGLFWWindow(info, window);
	}

	void DestroyWindow(const Window& window)
	{
		DestroyGLFWWindow(window);
	}

	bool PollEvents(const Window& window)
	{
		return PollGLFWEvents(window);
	}
}
