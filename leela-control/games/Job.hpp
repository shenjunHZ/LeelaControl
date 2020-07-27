//
// Created by root on 1/20/19.
//

#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/thread/thread.hpp>
#include <QtCore/QString>
#include "IJob.hpp"
#include "logger/Logger.hpp"

namespace games
{
    struct LeelaJobCommand
    {
        std::string chassBoardID;
        std::string command;
        resultCallback callback;
    };

    class ILeela;

    class Job : public IJob
    {
    public:
        Job(const std::string& binary, const bool& enableGTPEngine, const bool& enableLeelaLog, spdlog::logger& logger);
        ~Job();
        void createJobParameter(const leelaStarLevel& level) override;
        void startGameLeela(const resultCallback& callback) override;
        void stopGameLeela(const resultCallback& callback) override;

        void inputGTPCommand(const std::string& chassBoardID, const std::string& command,
            const resultCallback& callback) override;

    private:
        bool isEnableGTPEngine();
/* leela zero support commands
* name    : print program name
* version : print program version
* time_setting 0 5 1 : 0 is the first time, and others mean that every 5 seconds you go down 1 step, and the actual setup takes 1 second, so it's roughly 4s
*/
        void inputDefaultCommand();
        void runGame(const resultCallback& callback);
        void gameA(); // for test

	private:
		int testTimes{0};

    private:
        bool m_enableGTPEngine;
        std::string m_binaryPath;
        bool m_enableLeelaLog;
        std::vector<std::string> m_defaultCommands;
        std::unique_ptr<ILeela> m_gameLeela;
        std::thread m_threadGame;
        std::condition_variable m_condition;
		mutable std::mutex m_mutex;
        boost::thread m_threadProcess;
		spdlog::logger& m_logger;
        std::string m_strOption;
        std::string m_strWeight;

        std::queue<LeelaJobCommand> m_commends;
    };
}
