#include "Core/Application.h"

#include "GUI/GUIContext.h"

namespace CGEngine
{
#define BIND_EVENT_FN(x) [this](auto&&... args) -> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

	Application::Application(const ApplicationCreateInfo& appInfo [[maybe_unused]] ) : m_logger(Logger("APP"))
	{
		m_sceneManager.AddScene(Scene({ "Default Scene", {}}));

		CreateWindow({ 800, 800,"Default Window" }, m_window);

		m_window.eventCallback = BIND_EVENT_FN(OnEvent);

		m_renderer = std::make_unique<Renderer>(Renderer({ GraphicsAPI::CG_OPENGL_API, m_window }));

		CreateGUIContext(m_window);
	}

	void Application::Run()
	{
		m_time.Start();

		const auto& defaultScene = m_sceneManager.DefaultScene();

		m_renderer->PreRender(defaultScene.GetMainCamera());

		while (!WindowClosed(m_window))
		{
			PollEvents();

			m_time.Update();

			BeginGUIFrame();

			CreateGUIWindow();

			EndGUIFrame();

			m_renderer->FirstPass();

			m_renderer->Render(m_time);

			m_renderer->SecondPass();

			DrawGUI();

			m_renderer->PostRender();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		if (event.handled) return;

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
	}

	bool Application::OnWindowResized(const WindowResizeEvent& event) const
	{
		m_renderer->ResizeProjection(m_sceneManager.DefaultScene().GetMainCamera());
		m_renderer->ResizeViewport(event.GetWidth(), event.GetHeight());

		return true;
	}

	void Application::Quit() const
	{
		DestroyGUIContext();
		DestroyWindow(m_window);
	}
}