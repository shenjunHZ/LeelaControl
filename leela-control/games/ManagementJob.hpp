#pragma once

#include <string>
#include <memory>
#include <map>
#include "games/IManagementJob.hpp"
#include "DefineView.hpp"

namespace Game
{
    class Job;
    using gameJobPrt = std::unique_ptr<IJob>;
    using namespace Define;

    class ManagementJob : public IManagementJob
    {
    public:
        ManagementJob(const std::string& binary);
        ~ManagementJob();

        void createJobLeela(const leelaStarLevel& level) override;
        const IJob& getJob(const leelaStarLevel& level) override;

    private:
        std::string m_binaryPath;
        bool m_enableGTPEngine;
        std::map<leelaStarLevel, gameJobPrt> m_gameJobs;
    };
}