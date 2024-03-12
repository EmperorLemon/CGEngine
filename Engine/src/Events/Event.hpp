#pragma once

#include <functional>

namespace CGEngine
{
#define BIT(x) (1 << (x))
	// Credit to https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Events/Event.h

	enum class EventType : uint8_t
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_MOVED,
		MOUSE_MOVED, MOUSE_SCROLLED,
		MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED
	};

	enum EventCategory : uint8_t
	{
		NONE = 0,
		EVENT_CATEGORY_WINDOW = BIT(0),
		EVENT_CATEGORY_INPUT = BIT(1),
		EVENT_CATEGORY_KEYBOARD = BIT(2),
		EVENT_CATEGORY_MOUSE = BIT(3),
		EVENT_CATEGORY_MOUSE_BUTTON = BIT(4)
	};

#define EVENT_CLASS_TYPE(type)													\
	static EventType GetStaticType() { return EventType::type; }				\
	virtual EventType GetEventType() const override { return GetStaticType(); } \
	virtual const char* GetName() const override { return #type; }				\

#define EVENT_CLASS_CATEGORY(category)									   \
	virtual uint8_t GetCategoryFlags() const override { return category; } \

	class Event
	{
	public:
		[[nodiscard]] virtual EventType   GetEventType()  const = 0;
		[[nodiscard]] virtual const char* GetName()  const = 0;
		[[nodiscard]] virtual uint8_t GetCategoryFlags()  const = 0;

		bool handled = false;
	};

	class EventDispatcher
	{
		template <typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		explicit EventDispatcher(Event& event) : m_event(event) {}

		template <typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.handled |= func(static_cast<T&>(m_event));
				return true;
			}

			return false;
		}
	private:
		Event& m_event;
	};
}