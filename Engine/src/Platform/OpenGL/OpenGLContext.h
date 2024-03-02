#pragma once

#include <memory>

namespace CGEngine
{
	struct Window;
	struct DeviceProperties;
}

namespace CGEngine::OpenGL
{
	void CreateContext(const std::shared_ptr<Window>& window);
	void QueryDeviceProperties(DeviceProperties& properties);

	void SwapBuffers(const std::shared_ptr<Window>& window);
}