#pragma once
#include <string>
#include <time.h>
#include <QtCore/QString>
#include "configurations/DefineView.hpp"

namespace
{
    using VersionTuple = std::tuple<int, int, int>;
} // namespace

namespace games
{
using namespace configurations;
class ILeela
{
public:
    virtual bool gameStart(const VersionTuple& minVersion) = 0;
    virtual bool gameDown() = 0;
    virtual bool sendGtpCommand(QString cmd, std::string& result) = 0;

    virtual ~ILeela() = default;

protected:
    virtual std::string getCurrentTime()
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
        return std::string{ cTime };
    }

    void recordError(const errorInfo& error)
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
};

}  // namespace games