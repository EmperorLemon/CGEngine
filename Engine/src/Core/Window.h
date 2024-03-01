#pragma once

#include "Base.h"

namespace CGEngine
{
	struct Window
	{
		std::string title;

		uint32_t width = 0;
		uint32_t height = 0;

		void* window = nullptr;
		void* handle = nullptr;
	};

	struct WindowCreateInfo
	{
		std::string title;

		uint32_t width = 0;
		uint32_t height = 0;
	};

	void CreateWindow(const WindowCreateInfo& windowInfo, Window& window);
	void DestroyWindow(const Window& window);
	bool PollEvents(const Window& window);
}