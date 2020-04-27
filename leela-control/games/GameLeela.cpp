#ifndef WIN_ENV_RUN
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <time.h>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
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
 #ifdef WIN32
         m_strBinary.append(".exe");
 #endif
        boost::filesystem::path pathFile(m_strBinary);
        if (! (boost::filesystem::exists(pathFile) && boost::filesystem::is_regular_file(pathFile)))
        {
            recordError(errorInfo::ERROR_NO_LEELAZ);
        }
        m_cmdLine = m_strBinary + " " + opt + weights;
    }

    std::string GameLeela::getCurrentTime()
    {
        std::time_t currentTime(0);
        struct tm nowTime;
        std::time(&currentTime);
#ifdef WIN_ENV_RUN
        localtime_s(&nowTime, &currentTime);
#else
        localtime_r(&currentTime, &nowTime);
#endif // WIN_ENV_RUN

        char cTime[64];
        std::strftime(cTime, sizeof(cTime), "%Y-%m-%d %H:%M:%S", &nowTime);
        return std::string{cTime};
    }

    void GameLeela::recordError(const errorInfo& error)
    {
        switch (error)
        {
        case errorInfo::ERROR_NO_LEELAZ:
            LOG_ERROR_MSG("No 'leelaz' binary found.");
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
        //QTextStream(stdout) << "version:" << endl;
        waitForBytesWritten(-1);
        if (!waitReady()) 
        {
            recordError(errorInfo::ERROR_LAUNCH);
            exit(EXIT_FAILURE);
        }

        char readBuffer[256];
        int readCount = read(readBuffer, 256); // IO read, have some starting message need to read

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
        char readBuffer[256];
        // Eat double newline from GTP protocol
        if (!waitReady()) 
        {
            recordError(errorInfo::ERROR_PROCESS);
            return false;
        }
        auto readCount = readLine(readBuffer, 256);
        if (readCount < 0) 
        {
            recordError(errorInfo::ERROR_GTP);
            return false;
        }
        return true;
    }

    bool GameLeela::sendGtpCommand(QString cmd, std::string& result)
    {
        write(qPrintable(cmd.append("\n")));
        waitForBytesWritten(-1);
        if (!waitReady())
        {
            recordError(errorInfo::ERROR_PROCESS);
            return false;
        }

        char readBuffer[256];
        int readCount = readLine(readBuffer, 256);

        std::cout << "Debug: " << getCurrentTime() << std::endl;
        QTextStream(stdout) << "GTP: " << readBuffer << endl;
        if (readCount <= 0 || readBuffer[0] != '=')
        {
            recordError(errorInfo::ERROR_GTP);
            return false;
        }
        if (!eatNewLine())
        {
            recordError(errorInfo::ERROR_PROCESS);
            return false;
        }
        result = readBuffer;
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

        checkStatus(minVersion);
        QTextStream(stdout) << "Engine has started." << endl;
        for (auto command : m_commands) 
        {
            QTextStream(stdout) << QString::fromStdString(command) + ": " << endl;
            std::string result = "";
             if (!sendGtpCommand(QString::fromStdString(command), result))
             {
                 QTextStream(stdout) << "GTP failed on: " << QString::fromStdString(command) << endl;
                 exit(EXIT_FAILURE);
             }
        }
        //QTextStream(stdout) << "Thinking time set." << endl;
        return true;
    }

}