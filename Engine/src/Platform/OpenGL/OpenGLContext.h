#pragma once

namespace CGEngine
{
	struct Window;
	struct DeviceProperties;
}

namespace CGEngine::OpenGL
{
	void CreateContext(const Window& window);
	void QueryDeviceProperties(DeviceProperties& properties);

	void SwapBuffers(const Window& window);
}