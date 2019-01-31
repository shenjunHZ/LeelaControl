//
// Created by shenjun on 18-9-26.
//

#pragma once

#include <iostream>
#include <vector>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>

class LoggerConfiguration
{
public:
    static LoggerConfiguration& instance()
    {
        static LoggerConfiguration loggerConf;
        return loggerConf;
    }

    const std::vector<spdlog::sink_ptr>& getSinks() const
    {
        instance();
        return sinks;
    }

private:
    LoggerConfiguration()
    {
        try
        {
            constexpr unsigned loggingTreadPoolSize = 1;
            constexpr unsigned loggingThreadPoolQueueSize = 8192;
            spdlog::init_thread_pool(loggingThreadPoolQueueSize, loggingTreadPoolSize);
            setLoggingErrorHandler();
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
        }
    }

    ~LoggerConfiguration() = default;

    void setLoggingErrorHandler()
    {
        spdlog::set_error_handler([](const std::string& msg) { std::cerr << "SPDLOG ERROR:" << msg << std::endl; });
    }

private:
    static const std::vector<spdlog::sink_ptr> sinks;
};


