#pragma once

#include "Logger.hpp"
#include "Time.h"
#include "Window.h"

#include "Scene/SceneManager.h"
#include "Renderer/Renderer.h"

namespace CGEngine
{
	struct ApplicationCommandLineArgs
	{
		int    argc = 0;
		char** argv = nullptr;

		const char* operator[](const int index) const 
		{ 
			if (index < argc) return argv[index]; 

			return argv[0];
		}
	};

	struct ApplicationCreateInfo
	{
		std::string name = "Default CG Application";
		std::string directory;
		ApplicationCommandLineArgs cmdArgs;
	};

	class Application
	{
	public:
		explicit Application(const ApplicationCreateInfo& appInfo);

		void Run();
		void Quit() const;
	private:
		void OnEvent(Event& event);
		[[nodiscard]] bool OnWindowResized(const WindowResizeEvent& event);
		[[nodiscard]] bool OnFramebufferResized(const FramebufferResizeEvent& event) const;
	private:
		Logger m_logger;
		SceneManager m_sceneManager;

		Window m_window;
		Time m_time;
		std::unique_ptr<Renderer> m_renderer = nullptr;
	};
}