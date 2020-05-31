//
// Created by root on 1/20/19.
//
#include <memory>
#include <atomic>
#ifndef WIN_ENV_RUN
#include <unistd.h>
#endif
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "Job.hpp"
#include "GameLeela.hpp"
#include "LeelaCPUOnly.hpp"

namespace
{
    std::atomic_bool keep_running(true);
}

namespace games
{
    Job::Job(const std::string& binary, const bool& enableGTPEngine, const bool& enableLeelaLog, spdlog::logger& logger)
        : m_enableGTPEngine{enableGTPEngine}
        , m_binaryPath{binary}
        , m_gameLeela{nullptr}
        , m_enableLeelaLog{enableLeelaLog}
        , m_logger{logger}
        , m_strOption{""}
        , m_strWeight{""}
    {
        inputDefaultCommand();
    }

    Job::~Job()
    {
        if(m_threadGame.joinable())
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
       // m_defaultCommands.emplace_back("name");
       // m_defaultCommands.emplace_back("version");
        /*this is set so that the time value is not calculated by the machine
        as the development board resource limited*/
       // m_defaultCommands.emplace_back("time_settings 0 0 1");
    }

    void Job::createJobParameter(const leelaStarLevel& level)
    {
        if (isEnableGTPEngine())
        {
            m_strOption += "-g";
        }
        if(m_enableLeelaLog)
        {
            m_strOption += " --quiet --logfile " + m_binaryPath + "leelaz.log ";
        }
        m_strWeight = "-w ";

        switch (level)
        {
            case leelaStarLevel::STAR_LEVEL_1:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            case leelaStarLevel::STAR_LEVEL_2:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            case leelaStarLevel::STAR_LEVEL_3:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            case leelaStarLevel::STAR_LEVEL_4:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                return;
            case leelaStarLevel::STAR_LEVEL_5:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            case leelaStarLevel::STAR_LEVEL_6:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            case leelaStarLevel::STAR_LEVEL_7:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            case leelaStarLevel::STAR_LEVEL_8:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            case leelaStarLevel::STAR_LEVEL_9:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
                break;
            default:
                m_strWeight += m_binaryPath + "leelaz120.gz ";
        }
    }

    void Job::inputGTPCommand(const std::string& chassBoardID, const std::string& command,
        const resultCallback& callback)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_commends.push( {chassBoardID, command, callback} );
        m_condition.notify_one();
    }

    void Job::runGame(const resultCallback& callback)
    {
        std::tuple<int, int, int> version{ 0, 0, 1 };
        if (isEnableGTPEngine())
        {
            m_gameLeela = std::make_unique<GameLeela>(m_binaryPath + "leelaz", m_strOption, m_strWeight, m_defaultCommands, m_logger);
        }
        else
        {
            m_gameLeela = std::make_unique<LeelaCPUOnly>(m_binaryPath + "leelaz", m_strOption, m_strWeight, m_defaultCommands, m_logger);
        }
        /* QProcess need with GameLeela instantiation thread */
        m_gameLeela->gameStart(version);
        callback("=");

        //gameA();
        while(keep_running)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock, [this]()
            {
                return !m_commends.empty();
            });
            LeelaJobCommand& leelaJobCommend = m_commends.front();
            if ("dead" == leelaJobCommend.command)
            {
                LOG_DEBUG_MSG("Receive dead command, leela job will quit.");
                return;
            }
            std::string result = "";
            m_gameLeela->sendGtpCommand(QString::fromStdString(leelaJobCommend.command), result);
            if (leelaJobCommend.callback)
            {
                leelaJobCommend.callback(result);
            }
            m_commends.pop();
        }
    }

    void Job::startGameLeela(const resultCallback& callback)
    {
        LOG_INFO_MSG(m_logger, "Start leela game.");
        keep_running = true;

        m_threadGame = std::thread(&Job::runGame, this, callback);
    }

    void Job::stopGameLeela()
    {
        LOG_INFO_MSG(m_logger, "Stop leela game.");
        if (m_gameLeela)
        {
            m_gameLeela->gameDown();
        }

        m_strWeight.clear();
        keep_running = false;
    }

/***********************test************************/
    void Job::gameA()
    {
        std::cout<<"start game a"<<std::endl;
        //m_commends.push("quit");
        m_commends.push({ "0", "version", nullptr });
        m_commends.push({ "0", "boardsize 19", nullptr });
        m_commends.push({ "0", "clear_board", nullptr });

        m_commends.push({ "0", "play B A18", nullptr });
        m_commends.push({ "0", "lz-genmove_analyze w 100", nullptr });
    }

}