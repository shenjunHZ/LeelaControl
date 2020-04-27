#include "games/ManagementJob.hpp"
#include "Job.hpp"
#include "logger/Logger.hpp"

namespace games
{

    ManagementJob::ManagementJob(const std::string& binary, const bool& enableLeelaLog, spdlog::logger& logger)
        : m_binaryPath{binary}
        , m_enableGTPEngine{true}
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
        m_gameJobs[level] = std::make_unique<Job>(m_binaryPath, m_enableGTPEngine, m_enableLeelaLog, m_logger);
        m_gameJobs[level]->createJobParameter(level);
    }

    void ManagementJob::startJobLeela(const leelaStarLevel& level)
    {
        for (const auto& gameJob : m_gameJobs)
        {
            if (gameJob.first == level)
            {
                 gameJob.second->startGameLeela();
            }
        }
    }

    IJob& ManagementJob::getJob(const leelaStarLevel& level)
    {
         for (const auto& gameJob : m_gameJobs)
         {
             if (gameJob.first == level)
             {
                 return *(gameJob.second);
             }
         }
        createJobLeela(level);
        startJobLeela(level);
        return *m_gameJobs[level];
    }

}