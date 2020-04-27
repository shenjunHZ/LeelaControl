#pragma once
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <QtCore/QTextStream>

#define LEELA_CONFIG_PREFIX "leela"
#define ADDRESSER_PREFIX    "addresses"
#define LOGGER_PREFIX       "logger"
#define TCP_PREFIX          "tcp"

namespace configurations
{
    constexpr auto leelaPath            = LEELA_CONFIG_PREFIX ".leelaPath";
    constexpr auto enableLeelaLog       = LEELA_CONFIG_PREFIX ".enableLeelaLog";
    constexpr auto enableLeelaLevel_1   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_1";
    constexpr auto enableLeelaLevel_2   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_2";
    constexpr auto enableLeelaLevel_3   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_3";
    constexpr auto enableLeelaLevel_4   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_4";
    constexpr auto enableLeelaLevel_5   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_5";
    constexpr auto enableLeelaLevel_6   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_6";
    constexpr auto enableLeelaLevel_7   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_7";
    constexpr auto enableLeelaLevel_8   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_8";
    constexpr auto enableLeelaLevel_9   = LEELA_CONFIG_PREFIX ".enableLeelaLevel_9";
    constexpr auto waitLeelazStartTimes = LEELA_CONFIG_PREFIX ".waitLeelazStartTimes";

    constexpr auto serviceAddress = ADDRESSER_PREFIX ".serviceAddress";

    constexpr auto tcpServiceIp = TCP_PREFIX ".tcpServiceIp";
    constexpr auto tcpServicePort = TCP_PREFIX ".tcpServicePort";
    constexpr auto tcpBufferSize = TCP_PREFIX ".tcpBufferSize";
    constexpr auto tcpKeepAlive = TCP_PREFIX ".tcpKeepAlive";
    constexpr auto tcpKeepIdle = TCP_PREFIX ".tcpKeepIdle";
    constexpr auto tcpKeepInterval = TCP_PREFIX ".tcpKeepInterval";
    constexpr auto tcpKeepCount = TCP_PREFIX ".tcpKeepCount";

    constexpr auto loggerLevel    = LOGGER_PREFIX ".loggerLevel";
    constexpr auto maxFileSize    = LOGGER_PREFIX ".maxFileSize";
    constexpr auto maxRotateFiles = LOGGER_PREFIX ".maxRotateFiles";

    using AppConfiguration = boost::program_options::variables_map;

    AppConfiguration loadFromIniFile(const std::string& configFile);
}