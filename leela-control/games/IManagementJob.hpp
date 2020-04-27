#pragma once
#include "Job.hpp"

namespace games
{
    class IManagementJob
    {
    public:
        virtual ~IManagementJob() = default;
        virtual void createJobLeela(const leelaStarLevel& level) = 0;
        virtual void startJobLeela(const leelaStarLevel& level) = 0;
        virtual IJob& getJob(const leelaStarLevel& level) = 0;
    };
} // namespace games