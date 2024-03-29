#include "Platform/Windows/GLFWWindow.h"

#include "Core/Window.h"
#include "Core/Logger.hpp"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Request OpenGL version 4.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // Request OpenGL version x.6
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_SRGB_CAPABLE, true);

	window.title = windowInfo.title;

	window.width = windowInfo.width;
	window.height = windowInfo.height;

	window.window = glfwCreateWindow(windowInfo.width, windowInfo.height, windowInfo.title.data(), nullptr, nullptr);
	window.handle = nullptr;

	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(window.window), &window);

	glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(window.window), [](GLFWwindow* glfwWindow, const int width, const int height)
	{
		const auto winptr = static_cast<CGEngine::Window*>(glfwGetWindowUserPointer(glfwWindow));

		winptr->width = width;
		winptr->height = height;

		CGEngine::WindowResizeEvent event(width, height);
		winptr->eventCallback(event);
	});

	glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(window.window), [](GLFWwindow* glfwWindow, const int width, const int height)
	{
		const auto winptr = static_cast<CGEngine::Window*>(glfwGetWindowUserPointer(glfwWindow));

		CGEngine::FramebufferResizeEvent event(width, height);
		winptr->eventCallback(event);
	});

	s_WindowCount++;
}

void DestroyGLFWWindow(const CGEngine::Window& window)
{
	if (s_WindowCount >= 1)
	{
		glfwDestroyWindow(static_cast<GLFWwindow*>(window.window));
		s_WindowCount--;
	}

	if (s_WindowCount == 0)
	{
		glfwTerminate();
	}
}

bool GLFWWindowClosed(const CGEngine::Window& window)
{
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(window.window));
}

void PollGLFWEvents()
{
	glfwPollEvents();
}