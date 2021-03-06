#pragma once
#include <string>
#include <memory>
#include <map>
#include "games/IManagementJob.hpp"
#include "configurations/DefineView.hpp"

namespace games
{
    class Job;
    using gameJobPtr = std::unique_ptr<IJob>;
    using namespace configurations;

    class ManagementJob : public IManagementJob
    {
    public:
        ManagementJob(const std::string& binary, const bool& enableLeelaLog, spdlog::logger& logger,
            const configurations::AppConfiguration& config);
        ~ManagementJob();

        void createJobLeela(const leelaStarLevel& level) override;
        void startJobLeela(const leelaStarLevel& level, configurations::types::RespCallback callback) override;
        void stopGameLeela(const leelaStarLevel& level, configurations::types::RespCallback callback) override;
        boost::optional<IJob&> getJob(const leelaStarLevel& level) override;

    private:
        std::string m_binaryPath;
        bool m_enableGTPEngine;
        bool m_enableLeelaLog;
        std::map<leelaStarLevel, gameJobPtr> m_gameJobs;
        spdlog::logger& m_logger;
    };
}