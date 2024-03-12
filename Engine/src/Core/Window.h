#pragma once

#include <cstdint>
#include <string_view>

#include "Events/Event.hpp"

namespace CGEngine
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const int32_t width, const int32_t height) : m_width(width), m_height(height) {}

		[[nodiscard]] int32_t  GetWidth() const { return m_width; }
		[[nodiscard]] int32_t GetHeight() const { return m_height; }

		EVENT_CLASS_TYPE(WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_WINDOW)
	private:
		int32_t m_width = 0;
		int32_t m_height = 0;
	};

	struct Window
	{
		using EventCallbackFn = std::function<void(Event&)>;

		int32_t width = 0;
		int32_t height = 0;

		std::string_view title;

		void* window = nullptr;
		void* handle = nullptr;

		EventCallbackFn eventCallback;
	};

	struct WindowCreateInfo
	{
		int32_t width = 0;
		int32_t height = 0;

		std::string_view title;
	};

	void CreateWindow(const WindowCreateInfo& windowInfo, Window& window);
	void DestroyWindow(const Window& window);

	bool WindowClosed(const Window& window);
	void PollEvents();
}