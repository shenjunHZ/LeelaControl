#include "configurations/AppConfiguration.hpp"
#include "games/ManagementJob.hpp"
#include "Job.hpp"
#include "logger/Logger.hpp"

namespace
{
    bool getEnableGTPEngine(const configurations::AppConfiguration& config)
    {
        if (config.find(configurations::enableGTPEngine) != config.end())
        {
            return config[configurations::enableGTPEngine].as<bool>();
        }
        return false;
    }
} // namespace

namespace games
{

    ManagementJob::ManagementJob(const std::string& binary, const bool& enableLeelaLog, spdlog::logger& logger,
        const configurations::AppConfiguration& config)
        : m_binaryPath{binary}
        , m_enableGTPEngine{ getEnableGTPEngine(config) }
        , m_enableLeelaLog{enableLeelaLog}
        , m_logger{logger}
    {
        LOG_DEBUG_MSG("Start management job.");
        m_gameJobs.clear();
    }

    ManagementJob::~ManagementJob()
    {

    }

    void ManagementJob::createJobLeela(const leelaStarLevel& level)
    {
        LOG_DEBUG_MSG("Leela will created for level {}.", static_cast<int>(level));
        m_gameJobs[level] = std::make_unique<Job>(m_binaryPath, m_enableGTPEngine, m_enableLeelaLog, m_logger);
        m_gameJobs[level]->createJobParameter(level);
    }

    void ManagementJob::startJobLeela(const leelaStarLevel& level, configurations::types::RespCallback callback)
    {
        for (const auto& gameJob : m_gameJobs)
        {
            if (gameJob.first == level)
            {
                gameJob.second->startGameLeela(callback);
                 return;
            }
        }
        LOG_ERROR_MSG("Leela have not been created for level {}.", static_cast<int>(level));
    }

    void ManagementJob::stopGameLeela(const leelaStarLevel& level)
    {
        for (const auto& gameJob : m_gameJobs)
        {
            if (gameJob.first == level)
            {
                gameJob.second->stopGameLeela();
                gameJob.second->inputGTPCommand( "", "dead", nullptr );
                m_gameJobs.erase(level);
                return;
            }
        }
        LOG_ERROR_MSG("Can not find Leela to stop for level {}.", static_cast<int>(level));
    }

    boost::optional<IJob&> ManagementJob::getJob(const leelaStarLevel& level)
    {
         for (const auto& gameJob : m_gameJobs)
         {
             if (gameJob.first == level)
             {
                 return boost::make_optional<IJob&>(*(gameJob.second));
             }
         }
         LOG_ERROR_MSG("Can not get Leela to process for level {}.", static_cast<int>(level));
         return {};
    }

}