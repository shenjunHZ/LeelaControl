//
// Created by root on 1/20/19.
//
#include <memory>
#include <atomic>
#include <unistd.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "Job.hpp"
#include "GameLeela.hpp"

namespace
{
    std::atomic_bool keep_running(true);
}

namespace Game
{
    Job::Job(const std::string& binary, const bool& enableGTPEngine, const bool& enableLeelaLog)
        : m_enableGTPEngine{enableGTPEngine}
        , m_binaryPath{binary}
        , m_gameLeela{nullptr}
        , m_enableLeelaLog{enableLeelaLog}
    {
        inputDefaultCommand();
    }

    Job::~Job()
    {
        //if(m_threadGame.joinable())
        {
            m_threadGame.join();
        }
    }

    bool Job::isEnableGTPEngine()
    {
        return m_enableGTPEngine;
    }

    void Job::inputDefaultCommand()
    {
        m_defaultCommands.push_back("name");
        m_defaultCommands.push_back("version");
        /*this is set so that the time value is not calculated by the machine
        as the development board resource limited*/
        m_defaultCommands.push_back("time_setting 0 0 1");
    }

    void Job::createGameLeela(const leelaStarLevel& level)
    {
        std::string strOption = "";
        if (isEnableGTPEngine())
        {
            strOption += "-g ";
        }
        if(m_enableLeelaLog)
        {
            strOption += "--logfile " + m_binaryPath + "leelaz.log ";
        }
        std::string strWeight = "-w ";

        switch (level)
        {
            case leelaStarLevel::STAR_LEVEL_1:
                break;
            case leelaStarLevel::STAR_LEVEL_2:
                break;
            case leelaStarLevel::STAR_LEVEL_3:
                break;
            case leelaStarLevel::STAR_LEVEL_4:
                strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            case leelaStarLevel::STAR_LEVEL_5:
                break;
            case leelaStarLevel::STAR_LEVEL_6:
                break;
            case leelaStarLevel::STAR_LEVEL_7:
                break;
            case leelaStarLevel::STAR_LEVEL_8:
                break;
            case leelaStarLevel::STAR_LEVEL_9:
                break;
            default:
                strWeight += "leelaz120.gz ";
                break;
        }
        m_gameLeela = std::make_unique<GameLeela>(m_binaryPath + "leelaz", strOption, strWeight, m_defaultCommands);
    }

    void Job::runGame()
    {
        while(keep_running)
        {
            usleep(1000*10);
        }
    }

    void Job::startGameLeela()
    {
        std::tuple<int, int, int> version{ 0, 0, 1 };
        m_gameLeela->gameStart(version); // QProcess need with main thread

        boost::function<void()> fun = boost::bind(&Job::runGame, this);
        m_threadGame = boost::thread(fun);
    }
}