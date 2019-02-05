//
// Created by shenjun on 18-9-26.
//

#include "Logger.hpp"
#include <cerrno>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>

namespace
{
    const std::string logger_name = "Leela_Control.log";
}
namespace logger
{
    namespace
    {
        std::shared_ptr<spdlog::logger> createLogger(const std::string& name, spdlog::level::level_enum level)
        {
            const auto& sinks = LoggerConfiguration::instance().getSinks();
            // thread_pool() has queue size of 8192 and 1 worker thread
            auto logger = std::make_shared<spdlog::async_logger>(name, sinks.cbegin(), sinks.cend(), spdlog::thread_pool());
            logger->set_level(level);
            logger->set_pattern("[%H:%M:%S.%f][%n][%P][%t][%l] %v");
            logger->flush_on(spdlog::level::err);
            return logger;
        }
    } // namespace

    spdlog::logger& getLogger()
    {
        static auto logger = createLogger(logger_name, spdlog::level::trace);
        return *logger;
    }
} // namespace logger
