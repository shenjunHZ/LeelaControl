//
// Created by root on 1/20/19.
//

#pragma once

#include <vector>
#include <thread>
#include <boost/thread/thread.hpp>
#include "IJob.hpp"

namespace Game
{
    class GameLeela;

    class Job : public IJob
    {
    public:
        Job(const std::string& binary, const bool& enableGTPEngine, const bool& enableLeelaLog);
        ~Job();
        void createGameLeela(const leelaStarLevel& level) override;
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
        bool m_enableGTPEngine;
        std::string m_binaryPath;
        bool m_enableLeelaLog;
        std::vector<std::string> m_defaultCommands;
        std::unique_ptr<GameLeela> m_gameLeela;
        boost::thread m_threadGame;
    };
}
