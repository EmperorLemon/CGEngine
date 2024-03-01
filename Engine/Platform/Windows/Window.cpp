#include "Window.h"

#include <stdexcept>

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

static uint32_t s_WindowCount = 0;

void CreateWindow(const WindowCreateInfo& info, Window& window)
{
	if (s_WindowCount == 0)
	{
		if (!glfwInit())
			throw std::runtime_error("Unable to initialize glfw!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}