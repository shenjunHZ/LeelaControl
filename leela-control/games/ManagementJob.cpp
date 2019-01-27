#include "games/ManagementJob.hpp"
#include "Job.hpp"

namespace games
{

    ManagementJob::ManagementJob(const std::string& binary, const bool& enableLeelaLog)
        : m_binaryPath{binary}
        , m_enableGTPEngine{true}
        , m_enableLeelaLog{enableLeelaLog}
    {
        std::cout << "Debug: " << "start management job." << std::endl;
        m_gameJobs.clear();
    }

    ManagementJob::~ManagementJob()
    {

    }

    void ManagementJob::createJobLeela(const leelaStarLevel& level)
    {
        m_gameJobs[level] = std::make_unique<Job>(m_binaryPath, m_enableGTPEngine, m_enableLeelaLog);
        m_gameJobs[level]->createGameLeela(level);

        // to do
        m_gameJobs[level]->startGameLeela();
    }

    const IJob& ManagementJob::getJob(const leelaStarLevel& level)
    {
         for (const auto& gameJob : m_gameJobs)
         {
             if (gameJob.first == level)
             {
                 return *(gameJob.second);
             }
         }
        createJobLeela(level);
         return *m_gameJobs[level];
    }

}