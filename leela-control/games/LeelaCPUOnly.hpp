#pragma once
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <tuple>
#include <spdlog/logger.h>
#include "logger/Logger.hpp"
#include "ILeela.hpp"

namespace
{
    using VersionTuple = std::tuple<int, int, int>;
}

namespace games
{
    using namespace configurations;
    class LeelaCPUOnly : public ILeela, public QProcess
    {
    public:
        LeelaCPUOnly(const std::string& binary, const std::string& opt,
            const std::string& weights, const std::vector<std::string>& commands,
            spdlog::logger& logger);
        ~LeelaCPUOnly() = default;

        bool gameStart(const VersionTuple& minVersion) override;
        bool gameDown() override;
        bool sendGtpCommand(QString cmd, std::string& result) override;

    private:
        void checkStatus(const VersionTuple &minVersion);
        bool waitReady();
        bool eatNewLine();

    private:
        std::string m_strBinary;
        std::string m_cmdLine;
        std::vector<std::string> m_commands;
        spdlog::logger& m_logger;
    };
} // Game

