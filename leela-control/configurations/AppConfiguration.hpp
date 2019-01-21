#pragma once
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <QtCore/QTextStream>

#define LEELA_CONFIG_PREFIX "leela"

namespace Config
{
    constexpr auto leelaPath = LEELA_CONFIG_PREFIX ".leelaPath";
    constexpr auto enableLeelaLog = LEELA_CONFIG_PREFIX ".enableLeelaLog";

    using AppConfiguration = boost::program_options::variables_map;

    AppConfiguration loadFromIniFile(const std::string& configFile);
    void runApp(const Config::AppConfiguration& configParams, const Config::AppConfiguration& address);
}