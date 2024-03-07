#include "OpenGLContext.h"

#include "Core/Window.h"
#include "Core/Logger.hpp"
#include "Renderer/Context.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CGEngine::OpenGL
{
	static void DebugMessageCallback(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const GLchar* message, const void* userData)
	{
		const auto src_str = [source]
		{
			switch (source)
			{
			case GL_DEBUG_SOURCE_API:			  return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:	  return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY:	  return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION:	  return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER:			  return "OTHER";
			default: break;
			}

			return "UNKNOWN SOURCE";
		}();

		const auto type_str = [type]
		{
			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:				return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED BEHAVIOUR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED BEHAVIOUR";
			case GL_DEBUG_TYPE_PORTABILITY:		    return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE:			return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER:				return "MARKER";
			case GL_DEBUG_TYPE_OTHER:				return "OTHER";
			default: break;
			}

			return "UNKNOWN TYPE";
		}();

		const auto severity_str = [severity]
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW:			 return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM:		 return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH:		 return "HIGH";
			default: break;
			}

			return "UNKNOWN SEVERITY";
		}();

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			CG_TRACE("{0}, {1}, {2}, {3}: {4}", src_str, type_str, severity_str, id, message);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			CG_WARN("{0}, {1}, {2}, {3}: {4}", src_str, type_str, severity_str, id, message);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			CG_ERROR("{0}, {1}, {2}, {3}: {4}", src_str, type_str, severity_str, id, message);
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			CG_CRITICAL("{0}, {1}, {2}, {3}: {4}", src_str, type_str, severity_str, id, message);
			break;
		default: break;
		}
	}

	void CreateContext(const Window& window)
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(window.window));

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
			throw std::runtime_error("Failed to initialise GLAD!");

#ifdef CG_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugMessageCallback, nullptr);
#endif
	}

	void QueryDeviceProperties(DeviceProperties& properties)
	{
		properties.vendor      = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		properties.renderer    = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		properties.version     = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		properties.glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

		glGetIntegerv(GL_MAJOR_VERSION, &properties.versionMajor);
		glGetIntegerv(GL_MAJOR_VERSION, &properties.versionMinor);

		GLint numExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

		for (GLint i = 0; i < numExtensions; ++i)
		{
			const std::string_view extensionName = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));

			if (extensionName == "GL_ARB_bindless_texture")
			{
				properties.features.ARB_BINDLESS_TEXTURES = true;
			}

			if (extensionName == "GL_ARB_buffer_storage")
			{
				properties.features.ARB_BUFFER_STORAGE = true;
			}

			if (extensionName == "GL_ARB_direct_state_access")
			{
				properties.features.ARB_DSA = true;
			}
		}
	}

	void SwapBuffers(const Window& window)
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(window.window));
	}
}
