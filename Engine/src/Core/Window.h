#pragma once

#include "Base.h"

namespace CGEngine
{
	struct Window
	{
		int32_t width = 0;
		int32_t height = 0;

		std::string_view title;

		void* window = nullptr;
		void* handle = nullptr;
	};

	struct WindowCreateInfo
	{
		int32_t width = 0;
		int32_t height = 0;

		std::string_view title;
	};

	void CreateWindow(const WindowCreateInfo& windowInfo, Window& window);
	void DestroyWindow(const Window& window);

	bool PollEvents(const Window& window);
}