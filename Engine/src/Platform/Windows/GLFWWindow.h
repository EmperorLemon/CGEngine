#pragma once

namespace CGEngine
{
	struct Window;
	struct WindowCreateInfo;
}

void CreateGLFWWindow(const CGEngine::WindowCreateInfo& windowInfo, CGEngine::Window& window);
void DestroyGLFWWindow(const CGEngine::Window& window);

bool GLFWWindowClosed(const CGEngine::Window& window);

void PollGLFWEvents();