#pragma once

#include "Logger.hpp"
#include "Window.h"

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
		void Quit();
	private:
		Logger m_logger;

		Window m_window;
		std::unique_ptr<Renderer> m_renderer = nullptr;
	};
}