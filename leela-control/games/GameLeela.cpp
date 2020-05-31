#ifndef WIN_ENV_RUN
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <time.h>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include "games/GameLeela.hpp"
#include "configurations/DefineView.hpp"

namespace
{
#ifndef WIN_ENV_RUN
    constexpr int WAIT_GTP_INITIALIZE = 1000 * 1000 * 5;
#else
    /* in windows Sleep API in milliseconds*/
    constexpr int WAIT_GTP_INITIALIZE = 1000 * 5;
#endif
    constexpr int ReadBufferSize{256};
}
namespace games
{
    using namespace configurations;

    GameLeela::GameLeela(const std::string& binary, const std::string& opt, 
        const std::string& weights, const std::vector<std::string>& commands,
        spdlog::logger& logger)
        : QProcess()
        , m_strBinary{binary}
        , m_cmdLine{""}
        , m_commands{commands}
        , m_logger{logger}
    {
 #ifdef WIN_ENV_RUN
         m_strBinary.append(".exe");
 #endif
        boost::filesystem::path pathFile(m_strBinary);
        if (! (boost::filesystem::exists(pathFile) && boost::filesystem::is_regular_file(pathFile)))
        {
            recordError(errorInfo::ERROR_NO_LEELAZ);
        }
        m_cmdLine = m_strBinary + " " + opt + weights;
    }

    void GameLeela::recordError(const errorInfo& error)
    {
        switch (error)
        {
        case errorInfo::ERROR_NO_LEELAZ:
            LOG_ERROR_MSG("No leelaz binary found.");
            break;
        case errorInfo::ERROR_PROCESS_DIED:
            LOG_ERROR_MSG("leelaz process died.");
            break;
        case errorInfo::ERROR_LAUNCH:
            LOG_ERROR_MSG("Could not talk to engine after launching.");
            break;
        case errorInfo::ERROR_PROCESS:
            LOG_ERROR_MSG("The 'leelaz' process died unexpected.");
            break;
        case errorInfo::ERROR_GTP:
            LOG_ERROR_MSG("Error in GTP response.");
            break;
        }
    }

    void GameLeela::checkStatus(const VersionTuple &minVersion)
    {
#ifdef WIN_ENV_RUN
        Sleep(WAIT_GTP_INITIALIZE);
#else
        usleep(WAIT_GTP_INITIALIZE);
#endif
        write(qPrintable("version\n"));
        LOG_DEBUG_MSG("Send version gtp to leela.");
        waitForBytesWritten(-1);
        if (!waitReady()) 
        {
            recordError(errorInfo::ERROR_LAUNCH);
            exit(EXIT_FAILURE);
        }

        char readBuffer[ReadBufferSize];
        int readCount = readLine(readBuffer, sizeof(readBuffer)); // IO read, have some starting message need to read
        //If it is a GTP comment just print it and wait for the real answer
        //this happens with the winogard tuning
        if (readBuffer[0] == '#') 
        {
            readBuffer[readCount - 1] = 0;
            LOG_DEBUG_MSG("Read leela zero version Info: {}", readBuffer);
            if (!waitReady()) 
            {
                recordError(errorInfo::ERROR_PROCESS_DIED);
                exit(EXIT_FAILURE);
            }
            readCount = readLine(readBuffer, 256);
        }

        LOG_DEBUG_MSG("Read leela zero version Info: {}", readBuffer);
    }

    bool GameLeela::waitReady()
    {
        while (!canReadLine() && state() == QProcess::Running) 
        {
            waitForReadyRead(-1);
        }
        
        if (state() != QProcess::Running) 
        {
            return false;
        }
        return true;
    }

    bool GameLeela::eatNewLine()
    {
        char readBuffer[ReadBufferSize];
        // Eat double newline from GTP protocol
        if (!waitReady()) 
        {
            recordError(errorInfo::ERROR_PROCESS);
            return false;
        }
        auto readCount = readLine(readBuffer, sizeof(readBuffer));
        if (readCount < 0) 
        {
            LOG_WARNING_MSG("Read leela data empty.");
            recordError(errorInfo::ERROR_GTP);
            return false;
        }
        return true;
    }

    bool GameLeela::sendGtpCommand(QString cmd, std::string& result)
    {
        LOG_DEBUG_MSG("Time: {}, Send GTP command to leela: {}", getCurrentTime(), cmd.toStdString().c_str());
        write(qPrintable(cmd.append("\n")));
        waitForBytesWritten(-1);
        if (!waitReady())
        {
            recordError(errorInfo::ERROR_PROCESS);
            return false;
        }

        char readBuffer[ReadBufferSize];
        int readCount = readLine(readBuffer, sizeof(readBuffer));
        if (readCount <= 0 || readBuffer[0] != '=')
        {
            LOG_WARNING_MSG("Receive data from leela incorrect: {}", readBuffer);
            recordError(errorInfo::ERROR_GTP);
            return false;
        }
        if (!eatNewLine())
        {
            recordError(errorInfo::ERROR_PROCESS);
            return false;
        }
        LOG_DEBUG_MSG("Receive GTP response from leela zero: {}", readBuffer);
        result = readBuffer;
        const auto& pos = result.find("=");
        if (std::string::npos != pos)
        {
            result = result.substr(pos, result.size() - pos);
        }

        return true;
    }

    bool GameLeela::gameStart(const VersionTuple& minVersion)
    {
        start(QString::fromStdString(m_cmdLine)); // use QProcess
        LOG_DEBUG_MSG("Start command: {}", m_cmdLine.c_str());
        /*wait for process start*/
        if (!waitForStarted())
        {
            recordError(errorInfo::ERROR_NO_LEELAZ);
            return false;
        }
        int readCount = 0;
        do 
        {
            char readBuffer[ReadBufferSize];
            readCount = readLine(readBuffer, sizeof(readBuffer));
            LOG_DEBUG_MSG("Read leela zero Info: {}", readBuffer);
        } while (readCount > 0);

        checkStatus(minVersion);
        LOG_DEBUG_MSG("Engine has started.");
        for (auto command : m_commands) 
        {
            std::string result = "";
             if (!sendGtpCommand(QString::fromStdString(command), result))
             {
                 LOG_ERROR_MSG("GTP failed on: {}, result: {}", command, result);
                 continue;
             }
        }
        return true;
    }

    bool GameLeela::gameDown()
    {
        LOG_DEBUG_MSG("End leela engine.");
        terminate();
        return true;
    }

}