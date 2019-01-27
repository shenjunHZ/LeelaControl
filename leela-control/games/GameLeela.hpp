/*
* Copyright (C) jun shen
*/
#pragma once

#include <QtCore/QProcess>
#include <QtCore/QString>
#include <tuple>
#include "configurations/DefineView.hpp"

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
        */
        GameLeela(const std::string& binary, const std::string& opt,
            const std::string& weights, const std::vector<std::string>& commands);
        ~GameLeela() = default;

        bool gameStart(const VersionTuple& minVersion);
        bool sendGtpCommand(QString cmd);

    private:
        void recordError(const errorInfo& error);
        void checkStatus(const VersionTuple &minVersion);
        bool waitReady();
        bool eatNewLine();

    private:
        std::string m_strBinary;
        std::string m_cmdLine;
        std::vector<std::string> m_commands;
    };
} // Game

