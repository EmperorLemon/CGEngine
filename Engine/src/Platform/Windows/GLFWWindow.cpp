#include "Platform/Windows/GLFWWindow.h"
#include "Core/Window.h"

#include <stdexcept>

#include <GLFW/glfw3.h>

static uint32_t s_WindowCount = 0;

void CreateGLFWWindow(const CGEngine::WindowCreateInfo& windowInfo, CGEngine::Window& window)
{
	if (s_WindowCount == 0)
	{
		if (!glfwInit())
			throw std::runtime_error("Unable to initialize glfw!");
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // take advantage of multi-sample anti-aliasing

	window.title = windowInfo.title;

	window.width = windowInfo.width;
	window.height = windowInfo.height;

	window.window = glfwCreateWindow(windowInfo.width, windowInfo.height, windowInfo.title.data(), nullptr, nullptr);
	window.handle = nullptr;

	s_WindowCount++;

	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(window.window), &window);

	glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(window.window), [](GLFWwindow* glfwWindow, const int width, const int height)
	{
		const auto winptr = static_cast<CGEngine::Window*>(glfwGetWindowUserPointer(glfwWindow));

		winptr->width  = width;
		winptr->height = height;

		CGEngine::WindowResizeEvent event(width, height);
		winptr->windowEventCallback(event);
	});
}

void DestroyGLFWWindow(const CGEngine::Window& window)
{
	if (s_WindowCount >= 1)
	{
		glfwDestroyWindow(static_cast<GLFWwindow*>(window.window));
		s_WindowCount--;
	}
	else
		glfwTerminate();
}

bool PollGLFWEvents(const CGEngine::Window& window)
{
	if (s_WindowCount >= 1)
	{
		glfwPollEvents();

		return !glfwWindowShouldClose(static_cast<GLFWwindow*>(window.window));
	}

	return true;
}