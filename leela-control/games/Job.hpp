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
    class GameLeela;

    class Job : public IJob
    {
    public:
        Job(const std::string& binary, const bool& enableGTPEngine, const bool& enableLeelaLog, spdlog::logger& logger);
        ~Job();
        void createJobParameter(const configurations::leelaStarLevel& level) override;
        void startGameLeela() override;

    private:
        bool isEnableGTPEngine();
/* leela zero support commands
* name    : print program name
* version : print program version
* time_setting 0 5 1 : It means that every 5 seconds you go down 1 step, and the actual setup takes 1 second, so it's roughly 4s
*/
        void inputDefaultCommand();
        void runGame();

	private:
		void gameA();
		void gameB();
		int testTimes{0};

    private:
        bool m_enableGTPEngine;
        std::string m_binaryPath;
        bool m_enableLeelaLog;
        std::vector<std::string> m_defaultCommands;
        std::unique_ptr<GameLeela> m_gameLeela;
        boost::thread m_threadGame;
        std::condition_variable m_condition;
		mutable std::mutex m_mutex;
        boost::thread m_threadProcess;
		spdlog::logger& m_logger;
        std::string m_strOption;
        std::string m_strWeight;

        std::queue<QString> m_commends;
    };
}
