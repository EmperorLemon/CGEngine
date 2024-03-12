#include "Core/Application.h"

#include "GUI/GUIContext.h"

namespace CGEngine
{
	Application::Application(const ApplicationCreateInfo& appInfo [[maybe_unused]]) : m_logger(Logger("APP"))
	{
		m_sceneManager.AddScene(Scene({ "Default Scene" }));

		CreateWindow({ 800, 800,"Default Window" }, m_window);

		m_renderer = std::make_unique<Renderer>(Renderer({ GraphicsAPI::CG_OPENGL_API, m_window }));

		CreateGUIContext(m_window);
	}

	void Application::Run()
	{
		m_time.Start();

		m_renderer->PreRender(m_sceneManager.DefaultScene().GetMainCamera());

		while (!WindowClosed(m_window))
		{
			PollEvents();

			m_time.Update();

			BeginGUIFrame();

			CreateGUIWindow();

			EndGUIFrame();

			m_renderer->Render(m_time);

			DrawGUI();

			m_renderer->PostRender();
		}
	}

	void Application::Quit() 
	{
		DestroyGUIContext();
		DestroyWindow(m_window);
	}
}