#include <unistd.h>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include "games/GameLeela.hpp"
#include "configurations/DefineView.hpp"

namespace
{
    constexpr int WAIT_GTP_INITIALIZE = 1000 * 1000 * 20;
}
namespace games
{
    using namespace configurations;

    GameLeela::GameLeela(const std::string& binary, const std::string& opt, 
        const std::string& weights, const std::vector<std::string>& commands)
        : QProcess()
        , m_strBinary{binary}
        , m_cmdLine{""}
        , m_commands{commands}
    {
 #ifdef WIN32
         m_strBinary.append(".exe");
 #endif
        boost::filesystem::path pathFile(m_strBinary);
        if (! (boost::filesystem::exists(pathFile) && boost::filesystem::is_regular_file(pathFile)))
        {
            // to do log
            recordError(errorInfo::ERROR_NO_LEELAZ);
        }
        m_cmdLine = m_strBinary + " " + opt + weights;
    }

    void GameLeela::recordError(const errorInfo& error)
    {
        // to do log
        QTextStream(stdout) << "ERROR:";
        switch (error)
        {
        case errorInfo::ERROR_NO_LEELAZ:
            QTextStream(stdout) << "No 'leelaz' binary found." << endl;
            break;
        case errorInfo::ERROR_LAUNCH:
            QTextStream(stdout) << "Could not talk to engine after launching." << endl;
            break;
        case errorInfo::ERROR_PROCESS:
            QTextStream(stdout) << "The 'leelaz' process died unexpected." << endl;
            break;
        case errorInfo::ERROR_GTP:
            QTextStream(stdout) << "Error in GTP response." << endl;
            break;
        }
    }

    void GameLeela::checkStatus(const VersionTuple &minVersion)
    {
        usleep(WAIT_GTP_INITIALIZE);
        //write(qPrintable("version\n"));
        //QTextStream(stdout) << "version:" << endl;
        waitForBytesWritten(-1);
        if (!waitReady()) 
        {
            recordError(errorInfo::ERROR_LAUNCH);
            exit(EXIT_FAILURE);
        }
        char readBuffer[256];
        int readCount = read(readBuffer, 256); // IO read, have some starting message need to read
        std::cout << readBuffer << std::endl;
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

    bool GameLeela::sendGtpCommand(QString cmd)
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
        return true;
    }

    bool GameLeela::gameStart(const VersionTuple& minVersion)
    {
        start(QString::fromStdString(m_cmdLine)); // use QProcess
        std::cout << "cmd line========" << m_cmdLine.c_str() << std::endl;
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
             if (!sendGtpCommand(QString::fromStdString(command)))
             {
                 QTextStream(stdout) << "GTP failed on: " << QString::fromStdString(command) << endl;
                 exit(EXIT_FAILURE);
             }
        }
        //QTextStream(stdout) << "Thinking time set." << endl;
        return true;
    }

}