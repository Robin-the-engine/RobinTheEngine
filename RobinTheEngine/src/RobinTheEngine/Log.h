#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace RTE
{
	class RTE_API Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger> GetLogger(std::string name);
		static std::shared_ptr<spdlog::logger> GetCoreLogger();
		[[deprecated("Use GetLogger with meaningful name instead")]]
		static std::shared_ptr<spdlog::logger> GetClientLogger();

	private:
		static std::shared_ptr<spdlog::logger> CreateAndGetLogger(std::string name);
		static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> shared_file_sink;
		static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> shared_console_sink;
	};

}



//core log macros
#define RTE_CORE_TRACE(...)		::RTE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RTE_CORE_INFO(...)		::RTE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RTE_CORE_WARN(...)		::RTE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RTE_CORE_ERROR(...)		::RTE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RTE_CORE_FATAL(...)		::RTE::Log::GetCoreLogger()->critical(__VA_ARGS__)

//deprecated client log macros
#define RTE_TRACE(...)		::RTE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RTE_INFO(...)		::RTE::Log::GetClientLogger()->info(__VA_ARGS__)
#define RTE_WARN(...)		::RTE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RTE_ERROR(...)		::RTE::Log::GetClientLogger()->error(__VA_ARGS__)
#define RTE_FATAL(...)		::RTE::Log::GetClientLogger()->critical(__VA_ARGS__)
