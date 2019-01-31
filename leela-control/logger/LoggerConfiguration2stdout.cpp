//
// Created by shenjun on 18-9-27.
//
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <vector>
#include "LoggerConfiguration.hpp"

namespace
{
    const std::string LEELA_CONTROL_LOG = "leela-control.log";
}
const std::vector<spdlog::sink_ptr> LoggerConfiguration::sinks = {
        std::make_shared<spdlog::sinks::stdout_sink_mt>(),
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(LEELA_CONTROL_LOG)};
