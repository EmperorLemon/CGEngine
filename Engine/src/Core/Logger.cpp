#include "Core/Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace CGEngine
{
	std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;

	Logger::Logger(const char* name)
	{
		m_logger = spdlog::stdout_color_mt(name);
		m_logger->set_level(spdlog::level::trace);
	}

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] - %v%$");

		s_logger = spdlog::stdout_color_mt("ENGINE");
		s_logger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Logger::GetLogger()
	{
		return s_logger;
	}
}