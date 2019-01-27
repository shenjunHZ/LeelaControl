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

namespace games
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
        m_defaultCommands.push_back("time_settings 0 0 1");
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
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock, [this]()
            {
                return !m_commends.empty();
            });
            QString& commend = m_commends.front();
            m_gameLeela->sendGtpCommand(commend);
            m_commends.pop();

            if(m_commends.empty())
            {
                gameB();
                usleep(1000 * 10);
            }
        }
    }

    void Job::startGameLeela()
    {
        gameA();
        std::tuple<int, int, int> version{ 0, 0, 1 };
        m_gameLeela->gameStart(version); // QProcess need with main thread

        boost::function<void()> fun = boost::bind(&Job::runGame, this);
        m_threadGame = boost::thread(fun);
    }

/***********************just for test************************/
    void Job::gameA()
    {
        m_commends.push("quit");
        m_commends.push("version");
        m_commends.push("boardsize 19");
        m_commends.push("clear_board");

        m_commends.push("play B A19");
        m_commends.push("play W A17");
        m_commends.push("play B A15");
        m_commends.push("play W A13");
        m_commends.push("play B A11");
        m_commends.push("play W A9");
        m_commends.push("play B A7");
        m_commends.push("play W A5");
        m_commends.push("play B A3");
        m_commends.push("play W A1");

        m_commends.push("play B B19");
        m_commends.push("play W B17");
        m_commends.push("play B B15");
        m_commends.push("play W B13");
        m_commends.push("play B B11");
        m_commends.push("play W B9");
        m_commends.push("play B B7");
        m_commends.push("play W B5");
        m_commends.push("play B B3");
        m_commends.push("play W B1");

        m_commends.push("play B C19");
        m_commends.push("play W C17");
        m_commends.push("play B C15");
        m_commends.push("play W C13");
        m_commends.push("play B C11");
        m_commends.push("play W C9");
        m_commends.push("play B C7");
        m_commends.push("play W C5");
        m_commends.push("play B C3");
        m_commends.push("play W C1");

        m_commends.push("play B D19");
        m_commends.push("play W D17");
        m_commends.push("play B D15");
        m_commends.push("play W D13");
        m_commends.push("play B D11");
        m_commends.push("play W D9");
        m_commends.push("play B D7");
        m_commends.push("play W D5");
        m_commends.push("play B D3");
        m_commends.push("play W D1");

        m_commends.push("play B E19");
        m_commends.push("play W E17");
        m_commends.push("play B E15");
        m_commends.push("play W E13");
        m_commends.push("play B E11");
        m_commends.push("play W E9");
        m_commends.push("play B E7");
        m_commends.push("play W E5");
        m_commends.push("play B E3");
        m_commends.push("play W E1");

        m_commends.push("play B F19");
        m_commends.push("play W F17");
        m_commends.push("play B F15");
        m_commends.push("play W F13");
        m_commends.push("play B F11");
        m_commends.push("play W F9");
        m_commends.push("play B F7");
        m_commends.push("play W F5");
        m_commends.push("play B F3");
        m_commends.push("play W F1");

        m_commends.push("play B G19");
        m_commends.push("play W G17");
        m_commends.push("play B G15");
        m_commends.push("play W G13");
        m_commends.push("play B G11");
        m_commends.push("play W G9");
        m_commends.push("play B G7");
        m_commends.push("play W G5");
        m_commends.push("play B G3");
        m_commends.push("play W G1");

        m_commends.push("play B H19");
        m_commends.push("play W H17");
        m_commends.push("play B H15");
        m_commends.push("play W H13");
        m_commends.push("play B H11");
        m_commends.push("play W H9");
        m_commends.push("play B H7");
        m_commends.push("play W H5");
        m_commends.push("play B H3");
        m_commends.push("play W H1");

        m_commends.push("play B I19");
        m_commends.push("play W I17");
        m_commends.push("play B I15");
        m_commends.push("play W I13");
        m_commends.push("play B I11");
        m_commends.push("play W I9");
        m_commends.push("play B I7");
        m_commends.push("play W I5");
        m_commends.push("play B I3");
        m_commends.push("play W I1");

        m_commends.push("play B J19");
        m_commends.push("play W J17");
        m_commends.push("play B J15");
        m_commends.push("play W J13");
        m_commends.push("play B J11");
        m_commends.push("play W J9");
        m_commends.push("play B J7");
        m_commends.push("play W J5");
        m_commends.push("play B J3");
        m_commends.push("play W J1");

        m_commends.push("play B K19");
        m_commends.push("play W K17");
        m_commends.push("play B K15");
        m_commends.push("play W K13");
        m_commends.push("play B K11");
        m_commends.push("play W K9");
        m_commends.push("play B K7");
        m_commends.push("play W K5");
        m_commends.push("play B K3");
        m_commends.push("play W K1");

        m_commends.push("play B L19");
        m_commends.push("play W L17");
        m_commends.push("play B L15");
        m_commends.push("play W L13");
        m_commends.push("play B L11");
        m_commends.push("play W L9");
        m_commends.push("play B L7");
        m_commends.push("play W L5");
        m_commends.push("play B L3");
        m_commends.push("play W L1");

        m_commends.push("play B M19");
        m_commends.push("play W M17");
        m_commends.push("play B M15");
        m_commends.push("play W M13");
        m_commends.push("play B M11");
        m_commends.push("play W M9");
        m_commends.push("play B M7");
        m_commends.push("play W M5");
        m_commends.push("play B M3");
        m_commends.push("play W M1");

        m_commends.push("play B N19");
        m_commends.push("play W N17");
        m_commends.push("play B N15");
        m_commends.push("play W N13");
        m_commends.push("play B N11");
        m_commends.push("play W N9");
        m_commends.push("play B N7");
        m_commends.push("play W N5");
        m_commends.push("play B N3");
        m_commends.push("play W N1");

        m_commends.push("play B O19");
        m_commends.push("play W O17");
        m_commends.push("play B O15");
        m_commends.push("play W O13");
        m_commends.push("play B O11");
        m_commends.push("play W O9");
        m_commends.push("play B O7");
        m_commends.push("play W O5");
        m_commends.push("play B O3");
        m_commends.push("play W O1");

        m_commends.push("play B P19");
        m_commends.push("play W P17");
        m_commends.push("play B P15");
        m_commends.push("play W P13");
        m_commends.push("play B P11");
        m_commends.push("play W P9");
        m_commends.push("play B P7");
        m_commends.push("play W P5");
        m_commends.push("play B P3");
        m_commends.push("play W P1");

        m_commends.push("play B Q19");
        m_commends.push("play W Q17");
        m_commends.push("play B Q15");
        m_commends.push("play W Q13");
        m_commends.push("play B Q11");
        m_commends.push("play W Q9");
        m_commends.push("play B Q7");
        m_commends.push("play W Q5");
        m_commends.push("play B Q3");
        m_commends.push("play W Q1");

        m_commends.push("play B R19");
        m_commends.push("play W R17");
        m_commends.push("play B R15");
        m_commends.push("play W R13");
        m_commends.push("play B R11");
        m_commends.push("play W R9");
        m_commends.push("play B R7");
        m_commends.push("play W R5");
        m_commends.push("play B R3");
        m_commends.push("play W R1");

        m_commends.push("play B S19");
        m_commends.push("play W S17");
        m_commends.push("play B S15");
        m_commends.push("play W S13");
        m_commends.push("play B S11");
        m_commends.push("play W S9");
        m_commends.push("play B S7");
        m_commends.push("play W S5");
        m_commends.push("play B S3");
        m_commends.push("play W S1");

        m_commends.push("play B T19");
        m_commends.push("play W T17");
        m_commends.push("play B T15");
        m_commends.push("play W T13");
        m_commends.push("play B T11");
        m_commends.push("play W T9");
        m_commends.push("play B T7");
        m_commends.push("play W T5");
        m_commends.push("play B T3");
        m_commends.push("play W T1");

        m_commends.push("play B A18");
        m_commends.push("lz-genmove_analyze w 100");
    }

    void Job::gameB()
    {
        m_commends.push("quit");
        m_commends.push("version");
        m_commends.push("boardsize 19");
        m_commends.push("clear_board");

        m_commends.push("play B A19");
        m_commends.push("play W A17");
        m_commends.push("play B A15");
        m_commends.push("play W A13");
        m_commends.push("play B A11");
        m_commends.push("play W A9");
        m_commends.push("play B A7");
        m_commends.push("play W A5");
        m_commends.push("play B A3");
        m_commends.push("play W A1");

        m_commends.push("play B B19");
        m_commends.push("play W B17");
        m_commends.push("play B B15");
        m_commends.push("play W B13");
        m_commends.push("play B B11");
        m_commends.push("play W B9");
        m_commends.push("play B B7");
        m_commends.push("play W B5");
        m_commends.push("play B B3");
        m_commends.push("play W B1");

        m_commends.push("play B C19");
        m_commends.push("play W C17");
        m_commends.push("play B C15");
        m_commends.push("play W C13");
        m_commends.push("play B C11");
        m_commends.push("play W C9");
        m_commends.push("play B C7");
        m_commends.push("play W C5");
        m_commends.push("play B C3");
        m_commends.push("play W C1");

        m_commends.push("play B D19");
        m_commends.push("play W D17");
        m_commends.push("play B D15");
        m_commends.push("play W D13");
        m_commends.push("play B D11");
        m_commends.push("play W D9");
        m_commends.push("play B D7");
        m_commends.push("play W D5");
        m_commends.push("play B D3");
        m_commends.push("play W D1");

        m_commends.push("play B E19");
        m_commends.push("play W E17");
        m_commends.push("play B E15");
        m_commends.push("play W E13");
        m_commends.push("play B E11");
        m_commends.push("play W E9");
        m_commends.push("play B E7");
        m_commends.push("play W E5");
        m_commends.push("play B E3");
        m_commends.push("play W E1");

        m_commends.push("play B F19");
        m_commends.push("play W F17");
        m_commends.push("play B F15");
        m_commends.push("play W F13");
        m_commends.push("play B F11");
        m_commends.push("play W F9");
        m_commends.push("play B F7");
        m_commends.push("play W F5");
        m_commends.push("play B F3");
        m_commends.push("play W F1");

        m_commends.push("play B G19");
        m_commends.push("play W G17");
        m_commends.push("play B G15");
        m_commends.push("play W G13");
        m_commends.push("play B G11");
        m_commends.push("play W G9");
        m_commends.push("play B G7");
        m_commends.push("play W G5");
        m_commends.push("play B G3");
        m_commends.push("play W G1");

        m_commends.push("play B H19");
        m_commends.push("play W H17");
        m_commends.push("play B H15");
        m_commends.push("play W H13");
        m_commends.push("play B H11");
        m_commends.push("play W H9");
        m_commends.push("play B H7");
        m_commends.push("play W H5");
        m_commends.push("play B H3");
        m_commends.push("play W H1");

        m_commends.push("play B I19");
        m_commends.push("play W I17");
        m_commends.push("play B I15");
        m_commends.push("play W I13");
        m_commends.push("play B I11");
        m_commends.push("play W I9");
        m_commends.push("play B I7");
        m_commends.push("play W I5");
        m_commends.push("play B I3");
        m_commends.push("play W I1");

        m_commends.push("play B J19");
        m_commends.push("play W J17");
        m_commends.push("play B J15");
        m_commends.push("play W J13");
        m_commends.push("play B J11");
        m_commends.push("play W J9");
        m_commends.push("play B J7");
        m_commends.push("play W J5");
        m_commends.push("play B J3");
        m_commends.push("play W J1");

        m_commends.push("play B K19");
        m_commends.push("play W K17");
        m_commends.push("play B K15");
        m_commends.push("play W K13");
        m_commends.push("play B K11");
        m_commends.push("play W K9");
        m_commends.push("play B K7");
        m_commends.push("play W K5");
        m_commends.push("play B K3");
        m_commends.push("play W K1");

        m_commends.push("play B L19");
        m_commends.push("play W L17");
        m_commends.push("play B L15");
        m_commends.push("play W L13");
        m_commends.push("play B L11");
        m_commends.push("play W L9");
        m_commends.push("play B L7");
        m_commends.push("play W L5");
        m_commends.push("play B L3");
        m_commends.push("play W L1");

        m_commends.push("play B M19");
        m_commends.push("play W M17");
        m_commends.push("play B M15");
        m_commends.push("play W M13");
        m_commends.push("play B M11");
        m_commends.push("play W M9");
        m_commends.push("play B M7");
        m_commends.push("play W M5");
        m_commends.push("play B M3");
        m_commends.push("play W M1");

        m_commends.push("play B N19");
        m_commends.push("play W N17");
        m_commends.push("play B N15");
        m_commends.push("play W N13");
        m_commends.push("play B N11");
        m_commends.push("play W N9");
        m_commends.push("play B N7");
        m_commends.push("play W N5");
        m_commends.push("play B N3");
        m_commends.push("play W N1");

        m_commends.push("play B O19");
        m_commends.push("play W O17");
        m_commends.push("play B O15");
        m_commends.push("play W O13");
        m_commends.push("play B O11");
        m_commends.push("play W O9");
        m_commends.push("play B O7");
        m_commends.push("play W O5");
        m_commends.push("play B O3");
        m_commends.push("play W O1");

        m_commends.push("play B P19");
        m_commends.push("play W P17");
        m_commends.push("play B P15");
        m_commends.push("play W P13");
        m_commends.push("play B P11");
        m_commends.push("play W P9");
        m_commends.push("play B P7");
        m_commends.push("play W P5");
        m_commends.push("play B P3");
        m_commends.push("play W P1");

        m_commends.push("play B Q19");
        m_commends.push("play W Q17");
        m_commends.push("play B Q15");
        m_commends.push("play W Q13");
        m_commends.push("play B Q11");
        m_commends.push("play W Q9");
        m_commends.push("play B Q7");
        m_commends.push("play W Q5");
        m_commends.push("play B Q3");
        m_commends.push("play W Q1");

        m_commends.push("play B R19");
        m_commends.push("play W R17");
        m_commends.push("play B R15");
        m_commends.push("play W R13");
        m_commends.push("play B R11");
        m_commends.push("play W R9");
        m_commends.push("play B R7");
        m_commends.push("play W R5");
        m_commends.push("play B R3");
        m_commends.push("play W R1");

        m_commends.push("play B S19");
        m_commends.push("play W S17");
        m_commends.push("play B S15");
        m_commends.push("play W S13");
        m_commends.push("play B S11");
        m_commends.push("play W S9");
        m_commends.push("play B S7");
        m_commends.push("play W S5");
        m_commends.push("play B S3");
        m_commends.push("play W S1");

        m_commends.push("play B T19");
        m_commends.push("play W T17");
        m_commends.push("play B T15");
        m_commends.push("play W T13");
        m_commends.push("play B T11");
        m_commends.push("play W T9");
        m_commends.push("play B T7");
        m_commends.push("play W T5");
        m_commends.push("play B T3");
        m_commends.push("play W T1");

        m_commends.push("play B A16");
        m_commends.push("lz-genmove_analyze w 100");
    }
}