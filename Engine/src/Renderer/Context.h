#pragma once

#include <memory>
#include <string_view>

namespace CGEngine
{
	struct Window;

	struct DeviceFeatures
	{
		bool ARB_BUFFER_STORAGE = false;
		bool ARB_BINDLESS_TEXTURES = false;
		bool ARB_DSA = false;
	};

	struct DeviceProperties
	{
		std::string_view vendor;
		std::string_view renderer;
		std::string_view version;
		std::string_view glslVersion;
		int32_t versionMajor;
		int32_t versionMinor;

		DeviceFeatures features;
	};

	struct ContextCreateInfo
	{
		Window& window;
		DeviceProperties& properties;
	};

	void CreateContext(const ContextCreateInfo& contextInfo);
	void SwapBuffers(const Window& window);
}