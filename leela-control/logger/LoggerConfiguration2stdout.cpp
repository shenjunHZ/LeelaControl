//
// Created by shenjun on 18-9-27.
//
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "LoggerConfiguration.hpp"

namespace
{
    const std::string LEELA_CONTROL_LOG = "leela-control.log";
    const int maxFileSize = 3 * 1024 * 1024;
    const int maxRotateFiles = 3;
}
const std::vector<spdlog::sink_ptr> LoggerConfiguration::sinks = {
        std::make_shared<spdlog::sinks::rotating_file_sink_mt>(LEELA_CONTROL_LOG, maxFileSize, maxRotateFiles),
        std::make_shared<spdlog::sinks::stdout_sink_mt>()};
