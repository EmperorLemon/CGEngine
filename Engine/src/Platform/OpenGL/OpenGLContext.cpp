#include "OpenGLContext.h"

#include "Core/Window.h"
#include "Renderer/Context.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CGEngine::OpenGL
{
	void CreateContext(const std::shared_ptr<Window>& window)
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(window->window));

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
			throw std::runtime_error("Failed to initialise GLAD!");
	}

	void QueryDeviceProperties(DeviceProperties& properties)
	{
		properties.vendor      = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		properties.renderer    = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		properties.version     = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		properties.glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

		glGetIntegerv(GL_MAJOR_VERSION, &properties.versionMajor);
		glGetIntegerv(GL_MAJOR_VERSION, &properties.versionMinor);
	}

	void SwapBuffers(const std::shared_ptr<Window>& window)
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(window->window));
	}
}