#include "Context.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace CGEngine
{
	void CreateContext(const ContextCreateInfo& contextInfo)
	{
		OpenGL::CreateContext(contextInfo.window);
		OpenGL::QueryDeviceProperties(contextInfo.properties);
	}

	void SwapBuffers(const Window& window)
	{
		OpenGL::SwapBuffers(window);
	}
}