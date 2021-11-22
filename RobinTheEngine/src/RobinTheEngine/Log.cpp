#include "rtepch.h"
#include "Log.h"


namespace RTE
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger = spdlog::stdout_color_mt("RTE");
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger = spdlog::stdout_color_mt("APP");

	void Log::Init()
	{
		
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_level(spdlog::level::trace);
		
	}

}



