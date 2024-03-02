#include "Context.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace CGEngine
{
	void CreateContext(const ContextCreateInfo& contextInfo)
	{
		OpenGL::CreateContext(contextInfo.window);
		OpenGL::QueryDeviceProperties(contextInfo.properties);
	}

	void SwapBuffers(const std::shared_ptr<Window>& window)
	{
		OpenGL::SwapBuffers(window);
	}
}