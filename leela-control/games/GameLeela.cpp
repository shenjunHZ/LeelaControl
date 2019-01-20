#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include "games/GameLeela.hpp"
#include "DefineView.hpp"

namespace Game
{
    using namespace Define;

    GameLeela::GameLeela(const std::string& binary, const std::string& opt, 
        const std::string& weights, const std::vector<std::string>& commands)
        : QProcess()
        , m_strBinary{binary}
        , m_cmdLine{""}
        , m_commands{commands}
    {
// #ifdef WIN32
//         m_strBinary.append(".exe");
// #endif
        boost::filesystem::path pathFile(m_strBinary);
        if (! (boost::filesystem::exists(pathFile) && boost::filesystem::is_regular_file(pathFile)))
        {
            // to do log
            recordError(errorInfo::ERROR_NO_LEELAZ);

            //m_strBinary = m_strBinary.substr(2);
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

    void GameLeela::checkVersion(const VersionTuple &minVersion)
    {
        write(qPrintable("version\n"));
        waitForBytesWritten(-1);
        if (!waitReady()) 
        {
            recordError(errorInfo::ERROR_LAUNCH);
            exit(EXIT_FAILURE);
        }
        char readBuffer[256];
        int readCount = readLine(readBuffer, 256); // IO read
        // If it is a GTP comment just print it and wait for the real answer
        if (readBuffer[0] == '#') 
        {
            readBuffer[readCount - 1] = 0;
            QTextStream(stdout) << readBuffer << endl;
            if (!waitReady())
            {
                recordError(errorInfo::ERROR_PROCESS);
                exit(EXIT_FAILURE);
            }
            readCount = readLine(readBuffer, 256);
        }
        // We expect to read at last "=, space, something"
        if (readCount <= 3 || readBuffer[0] != '=') 
        {
            QTextStream(stdout) << "GTP: " << readBuffer << endl;
            recordError(errorInfo::ERROR_GTP);
            exit(EXIT_FAILURE);
        }
        QString version_buff(&readBuffer[2]);
        version_buff = version_buff.simplified();
        QStringList version_list = version_buff.split(".");
        if (version_list.size() < 2) 
        {
            QTextStream(stdout) << "Unexpected Leela Zero version: " << version_buff << endl;
            exit(EXIT_FAILURE);
        }
        if (version_list.size() < 3) 
        {
            version_list.append("0");
        }
        int versionCount = (version_list[0].toInt() - std::get<0>(minVersion)) * 10000;
        versionCount += (version_list[1].toInt() - std::get<1>(minVersion)) * 100;
        versionCount += version_list[2].toInt() - std::get<2>(minVersion);
        if (versionCount < 0) 
        {
            QTextStream(stdout)
                << "Leela version is too old, saw " << version_buff
                << " but expected "
                << std::get<0>(minVersion) << "."
                << std::get<1>(minVersion) << "."
                << std::get<2>(minVersion) << endl;
            QTextStream(stdout)
                << "Check https://github.com/gcp/leela-zero for updates." << endl;
            exit(EXIT_FAILURE);
        }
        if (!eatNewLine()) 
        {
            exit(EXIT_FAILURE);
        }
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

    bool GameLeela::gameStart(const VersionTuple& minVersion)
    {
        start(QString::fromStdString(m_cmdLine)); // use QProcess
        std::cout << "cmd line========" << m_cmdLine.c_str() << std::endl;
        if (!waitForStarted())
        {
            recordError(errorInfo::ERROR_NO_LEELAZ);
            return false;
        }
//        checkVersion(minVersion);
        QTextStream(stdout) << "Engine has started." << endl;
        for (auto command : m_commands) 
        {
            QTextStream(stdout) << QString::fromStdString(command) << endl;
//             if (!sendGtpCommand(command))
//             {
//                 QTextStream(stdout) << "GTP failed on: " << QString::fromStdString(command) << endl;
//                 exit(EXIT_FAILURE);
//             }
        }
        QTextStream(stdout) << "Thinking time set." << endl;
        return true;
    }

}