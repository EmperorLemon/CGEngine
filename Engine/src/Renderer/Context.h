#pragma once

#include <memory>
#include <string>

namespace CGEngine
{
	struct Window;

	struct DeviceProperties
	{
		std::string_view vendor;
		std::string_view renderer;
		std::string_view version;
		std::string_view glslVersion;
		int32_t versionMajor;
		int32_t versionMinor;
	};

	struct ContextCreateInfo
	{
		std::shared_ptr<Window>& window;
		DeviceProperties& properties;
	};

	void CreateContext(const ContextCreateInfo& contextInfo);
}