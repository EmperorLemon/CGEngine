#pragma once

#include "Base.h"

namespace CGEngine
{
	struct Window
	{
		uint32_t width = 0;
		uint32_t height = 0;

		std::string_view title;

		void* window = nullptr;
		void* handle = nullptr;
	};

	struct WindowCreateInfo
	{
		uint32_t width = 0;
		uint32_t height = 0;

		std::string_view title;
	};

	void CreateWindow(const WindowCreateInfo& windowInfo, Window& window);
	void DestroyWindow(const Window& window);
	bool PollEvents(const Window& window);
}