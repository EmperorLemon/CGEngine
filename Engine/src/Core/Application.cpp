#include "Core/Application.h"

namespace CGEngine
{
	Application::Application(const ApplicationCreateInfo& appInfo)
	{
		m_logger = Logger("APP");

		WindowCreateInfo createInfo;
		{
			createInfo.width = 800;
			createInfo.height = 600;
			createInfo.title = "Default Window";
		}

		CreateWindow(createInfo, m_window);
	}

	void Application::Run()
	{
		while (PollEvents(m_window))
		{
			
		}
	}

	void Application::Quit()
	{

	}
}