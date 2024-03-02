#include "Core/Application.h"

namespace CGEngine
{
	Application::Application(const ApplicationCreateInfo& appInfo)
	{
		m_logger = Logger("APP");

		CreateWindow({800, 600,"Default Window"}, m_window);
		m_renderer = Renderer({ GraphicsAPI::CG_OPENGL_API, m_window});
	}

	void Application::Run()
	{
		m_renderer.PreRender();

		while (PollEvents(m_window))
		{
			m_renderer.Render();
		}

		m_renderer.PostRender();
	}

	void Application::Quit()
	{

	}
}