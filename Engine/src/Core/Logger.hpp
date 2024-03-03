#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace CGEngine
{
	class Logger
	{
	public:

		Logger() = default;
		explicit Logger(const char* name);

		static void Init();
		static std::shared_ptr<spdlog::logger>& GetLogger();

		template <typename... Args>
		void Trace(const char* fmt, Args&&... args)
		{
			if (m_logger)
			{
				const auto& msg = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
				m_logger->trace(msg);
			}
			else
				throw std::runtime_error("Failed to log info!");
		}

		template <typename... Args>
		void Info(const char* fmt, Args&&... args)
		{
			if (m_logger)
			{
				const auto& msg = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
				m_logger->info(msg);
			}
			else
				throw std::runtime_error("Failed to log info!");
		}

		template <typename... Args>
		void Warn(const char* fmt, Args&&... args)
		{
			if (m_logger)
			{
				const auto& msg = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
				m_logger->warn(msg);
			}
			else
				throw std::runtime_error("Failed to log info!");
		}

		template <typename... Args>
		void Error(const char* fmt, Args&&... args)
		{
			if (m_logger)
			{
				const auto& msg = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
				m_logger->error(msg);
			}
			else
				throw std::runtime_error("Failed to log info!");
		}

		template <typename... Args>
		void Critical(const char* fmt, Args&&... args)
		{
			if (m_logger)
			{
				const auto& msg = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
				m_logger->critical(msg);
			}
			else
				throw std::runtime_error("Failed to log info!");
		}
	private:
		std::shared_ptr<spdlog::logger> m_logger = nullptr;
		static std::shared_ptr<spdlog::logger> s_logger;
	};

#define      CG_TRACE(...) CGEngine::Logger::GetLogger()->trace(__VA_ARGS__)
#define      CG_INFO(...)  CGEngine::Logger::GetLogger()->info(__VA_ARGS__)
#define      CG_WARN(...)  CGEngine::Logger::GetLogger()->warn(__VA_ARGS__)
#define     CG_ERROR(...)  CGEngine::Logger::GetLogger()->error(__VA_ARGS__)
#define  CG_CRITICAL(...)  CGEngine::Logger::GetLogger()->critical(__VA_ARGS__)
}