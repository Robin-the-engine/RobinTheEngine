#include "rtepch.h"
#include "Log.h"

namespace RTE
{
    const static std::string logDir = "logs/";

    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Log::shared_file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(logDir + "Engine.log");
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Log::shared_console_sink =
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

#ifndef DNDEBUG
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);
#else
        spdlog::set_level(spdlog::level::info);
        spdlog::flush_on(spdlog::level::err);
#endif
    }

    std::shared_ptr<spdlog::logger> Log::GetLogger(std::string name)
    {
        auto logger = spdlog::get(std::string(name));
        if (!logger) {
            return CreateAndGetLogger(name);
        }
        return logger;
    }

    std::shared_ptr<spdlog::logger> Log::GetCoreLogger()
    {
        return GetLogger("RTE");
    }

    std::shared_ptr<spdlog::logger> Log::GetClientLogger()
    {
        return GetLogger("APP");
    }

    std::shared_ptr<spdlog::logger> Log::CreateAndGetLogger(std::string name)
    {
        auto newLoggerFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logDir + name);
        std::vector<spdlog::sink_ptr> sinks = { shared_console_sink, shared_file_sink, newLoggerFileSink };
        auto newLogger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
        spdlog::register_logger(newLogger);
        return newLogger;
    }

}



