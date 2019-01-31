/*
* Copyright (C) jun shen
*/
#pragma once

#include <QtCore/QProcess>
#include <QtCore/QString>
#include <tuple>
#include <spdlog/logger.h>
#include "configurations/DefineView.hpp"
#include "logger/Logger.hpp"

namespace
{
    using VersionTuple = std::tuple<int, int, int>;
}

namespace games
{
    using namespace configurations;
    class GameLeela : public QProcess
    {
    public:
        /*
        * binary = "./leelaz"
        * commands= std::string("time_settings 0 0 1")
        * quit : exit leela zero
        */
        GameLeela(const std::string& binary, const std::string& opt,
            const std::string& weights, const std::vector<std::string>& commands,
            spdlog::logger& logger);
        ~GameLeela() = default;

        bool gameStart(const VersionTuple& minVersion);
        bool sendGtpCommand(QString cmd);

    private:
        void recordError(const errorInfo& error);
        void checkStatus(const VersionTuple &minVersion);
        bool waitReady();
        bool eatNewLine();
        std::string getCurrentTime();

    private:
        std::string m_strBinary;
        std::string m_cmdLine;
        std::vector<std::string> m_commands;
        spdlog::logger& m_logger;
    };
} // Game

