#include "Core/Application.h"

namespace CGEngine
{
	Application::Application(const ApplicationCreateInfo& appInfo [[maybe_unused]]) : m_logger(Logger("APP"))
	{
		m_sceneManager.AddScene(Scene({ "Default Scene" }));

		CreateWindow({ 800, 800,"Default Window" }, m_window);

		m_renderer = std::make_unique<Renderer>(Renderer({ GraphicsAPI::CG_OPENGL_API, m_window }));
	}

	void Application::Run()
	{
		m_renderer->PreRender(m_sceneManager.DefaultScene().GetCamera());

		while (PollEvents(m_window))
		{
			m_renderer->Render();
		}

		m_renderer->PostRender();
	}

	void Application::Quit()
	{

	}
}