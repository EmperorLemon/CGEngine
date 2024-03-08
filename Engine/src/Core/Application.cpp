#include "Core/Application.h"

#include "ECS/Entity/Entity.hpp"

namespace CGEngine
{
	Application::Application(const ApplicationCreateInfo& appInfo [[maybe_unused]] ) : m_logger(Logger("APP"))
	{
		CreateWindow({ 800, 600,"Default Window" }, m_window);

		m_renderer = std::make_unique<Renderer>(Renderer({ GraphicsAPI::CG_OPENGL_API, m_window }));
	}

	void Application::Run()
	{
		m_renderer->PreRender();

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